package structures;

import java.util.*;

/**
 * Encapsulates an interval tree.
 * 
 * @author runb-cs112
 */
public class IntervalTree {
	
	/**
	 * The root of the interval tree
	 */
	IntervalTreeNode root;
	
	/**
	 * Constructs entire interval tree from set of input intervals. Constructing the tree
	 * means building the interval tree structure and mapping the intervals to the nodes.
	 * 
	 * @param intervals Array list of intervals for which the tree is constructed
	 * @throws Exception 
	 */
	public IntervalTree(ArrayList<Interval> intervals) throws Exception {
		
		// make a copy of intervals to use for right sorting
		ArrayList<Interval> intervalsRight = new ArrayList<Interval>(intervals.size());
		for (Interval iv : intervals) {
			intervalsRight.add(iv);
		}
		
		// rename input intervals for left sorting
		ArrayList<Interval> intervalsLeft = intervals;
		
		// sort intervals on left and right end points
		Sorter.sortIntervals(intervalsLeft, 'l');
		Sorter.sortIntervals(intervalsRight,'r');
		
		// get sorted list of end points without duplicates
		ArrayList<Integer> sortedEndPoints = Sorter.getSortedEndPoints(intervalsLeft, intervalsRight);
		
		// build the tree nodes
		root = buildTreeNodes(sortedEndPoints);
		
		// map intervals to the tree nodes
		mapIntervalsToTree(intervalsLeft, intervalsRight);
	}
	
	/**
	 * Builds the interval tree structure given a sorted array list of end points.
	 * 
	 * @param endPoints Sorted array list of end points
	 * @return Root of the tree structure
	 */
	public static IntervalTreeNode buildTreeNodes(ArrayList<Integer> endPoints) {
		Queue<IntervalTreeNode> points = new Queue<IntervalTreeNode>();
		Queue<IntervalTreeNode> nodes = new Queue<IntervalTreeNode>();
		IntervalTreeNode temp1, temp2, temp3;
		for(Integer e : endPoints){
			points.enqueue(new IntervalTreeNode(e, e, e));
		}
		
		while(!points.isEmpty()){
			temp1 = points.dequeue();
			if(!points.isEmpty()){
				temp2 = points.dequeue(); temp3 = new IntervalTreeNode((temp1.maxSplitValue + temp2.minSplitValue) / 2, temp1.minSplitValue, temp2.maxSplitValue);
				temp3.leftChild = temp1; temp3.rightChild = temp2;
				nodes.enqueue(temp3);
			}else
				nodes.enqueue(temp1);
		}
		while(nodes.size() > 1){
			temp1 = nodes.dequeue();
			if(!nodes.isEmpty()){
				temp2 = nodes.dequeue(); temp3 = new IntervalTreeNode((temp1.maxSplitValue + temp2.minSplitValue) / 2, temp1.minSplitValue, temp2.maxSplitValue);
				temp3.leftChild = temp1; temp3.rightChild = temp2;
				nodes.enqueue(temp3);
			}else
				nodes.enqueue(temp1);
			
		}
		return nodes.dequeue();
	}
	
	/**
	 * Maps a set of intervals to the nodes of this interval tree. 
	 * 
	 * @param leftSortedIntervals Array list of intervals sorted according to left endpoints
	 * @param rightSortedIntervals Array list of intervals sorted according to right endpoints
	 * @throws Exception 
	 */
	public void mapIntervalsToTree(ArrayList<Interval> leftSortedIntervals, ArrayList<Interval> rightSortedIntervals) throws Exception {
		ArrayList<Interval> tmp1 = new ArrayList<Interval>(), tmp2 = new ArrayList<Interval>();
		tmp1.addAll(leftSortedIntervals); tmp2.addAll(rightSortedIntervals);
		this.root.leftIntervals = new ArrayList<Interval>(); this.root.rightIntervals = new ArrayList<Interval>();
		int i = 0;
		while(i < tmp1.size()){
			if(tmp1.get(i).leftEndPoint <= this.root.splitValue && tmp1.get(i).rightEndPoint >= this.root.splitValue){
				this.root.leftIntervals.add(tmp1.get(i));
				tmp1.remove(i);
			}else
				i++;
		}
		i = 0;
		while(i < tmp2.size()){
			if(tmp2.get(i).leftEndPoint <= this.root.splitValue && tmp2.get(i).rightEndPoint >= this.root.splitValue){
				this.root.rightIntervals.add(tmp2.get(i));
				tmp2.remove(i);
			}else
				i++;
		}
		if(this.root.leftChild != null){
			populate(tmp1, tmp2, this.root.leftChild);
		}
		if(this.root.rightChild != null){
			populate(tmp1, tmp2, this.root.rightChild);
		}
	}
	private void populate(ArrayList<Interval> tmp1, ArrayList<Interval> tmp2, IntervalTreeNode start){
		if(start == null)
			return;
		start.leftIntervals = new ArrayList<Interval>(); start.rightIntervals = new ArrayList<Interval>();
		if((tmp1.isEmpty() && tmp2.isEmpty()))
			return;
		if(!tmp1.isEmpty()){
			int i = 0;
			while(i < tmp1.size()){
				if(tmp1.get(i).leftEndPoint <= start.splitValue && tmp1.get(i).rightEndPoint >= start.splitValue){
					start.leftIntervals.add(tmp1.get(i));
					tmp1.remove(i);
				}else
					i++;
			}
		}
		if(!tmp2.isEmpty()){
			int i = 0;
			while(i < tmp2.size()){
				if(tmp2.get(i).leftEndPoint <= start.splitValue && tmp2.get(i).rightEndPoint >= start.splitValue){
					start.rightIntervals.add(tmp2.get(i));
					tmp2.remove(i);
				}else
					i++;
			}
		}
		
		populate(tmp1, tmp2, start.leftChild);
		populate(tmp1,tmp2,start.rightChild);
	}
	
	/**
	 * Gets all intervals in this interval tree that intersect with a given interval.
	 * 
	 * @param q The query interval for which intersections are to be found
	 * @return Array list of all intersecting intervals; size is 0 if there are no intersections
	 */
	public ArrayList<Interval> findIntersectingIntervals(Interval q) {
		ArrayList<Interval> retval = new ArrayList<Interval>();
		if(q.leftEndPoint > this.root.maxSplitValue || q.rightEndPoint < this.root.minSplitValue )
			return retval;
		if(q.leftEndPoint > this.root.splitValue){// ignore left side of root
			for(int i = 0; i < this.root.rightIntervals.size(); i++){
				if(this.root.rightIntervals.get(i).leftEndPoint <= q.leftEndPoint && this.root.rightIntervals.get(i).rightEndPoint >= q.leftEndPoint)
					retval.add(this.root.rightIntervals.get(i));
			}
			retval = IntSect(q, retval, this.root.rightChild);
		}else if(q.rightEndPoint < this.root.splitValue){// ignore right side of root
			for(int i = 0; i < this.root.leftIntervals.size(); i++){
				if(this.root.leftIntervals.get(i).leftEndPoint <= q.rightEndPoint && this.root.leftIntervals.get(i).rightEndPoint >= q.rightEndPoint)
					retval.add(this.root.leftIntervals.get(i));
			}
			retval = IntSect(q, retval, this.root.leftChild);
		}else{// split is contained in interval
			retval.addAll(this.root.leftIntervals);
			retval.addAll(IntSect(q, retval, this.root.leftChild));
			retval.addAll(IntSect(q, retval, this.root.rightChild));
		}
		sortIntervals(retval, 'l');
		int i = 0, j = 1;
		while(i < retval.size()){
			j = i+1;
			while(j < retval.size()){
				if(retval.get(i).leftEndPoint == retval.get(j).leftEndPoint && retval.get(i).rightEndPoint == retval.get(j).rightEndPoint){
					retval.remove(j);
				}else
					j++;
			}
			i++;
		}
		return retval;
	}
	private ArrayList<Interval> IntSect(Interval q, ArrayList<Interval> retval, IntervalTreeNode child){
		if(q.leftEndPoint > child.maxSplitValue || q.rightEndPoint < child.minSplitValue )
			return retval;
		if(q.leftEndPoint > child.splitValue){// ignore left side of root
			if(child.rightIntervals.isEmpty())
				return retval;
			for(int i = 0; i < child.rightIntervals.size(); i++){
				if(child.rightIntervals.get(i).leftEndPoint <= q.leftEndPoint && child.rightIntervals.get(i).rightEndPoint >= q.leftEndPoint)
					retval.add(child.rightIntervals.get(i));
			}
			retval = IntSect(q, retval, child.rightChild);
			return retval;
		}else if(q.rightEndPoint < child.splitValue){// ignore right side of root
			if(child.leftIntervals.isEmpty())
				return retval;
			for(int i = 0; i < child.leftIntervals.size(); i++){
				if(child.leftIntervals.get(i).leftEndPoint <= q.rightEndPoint && child.leftIntervals.get(i).rightEndPoint >= q.rightEndPoint)
					retval.add(child.leftIntervals.get(i));
			}
			retval = IntSect(q, retval, child.leftChild);
			return retval;
		}else{// split is contained in interval
			boolean tmp1 = true, tmp2 = true;
			if(!child.leftIntervals.isEmpty()){
				tmp1 = false;
				for(int i = 0; i < child.leftIntervals.size(); i++){
					if(child.leftIntervals.get(i).leftEndPoint <= q.rightEndPoint && child.leftIntervals.get(i).rightEndPoint >= q.rightEndPoint)
						retval.add(child.leftIntervals.get(i));
					else if(child.leftIntervals.get(i).leftEndPoint > q.leftEndPoint && child.leftIntervals.get(i).rightEndPoint < q.rightEndPoint)
						retval.add(child.leftIntervals.get(i));
				}
			}
			if(!child.rightIntervals.isEmpty()){
				tmp2 = false;
				for(int i = 0; i < child.rightIntervals.size(); i++){
					if(child.rightIntervals.get(i).leftEndPoint <= q.leftEndPoint && child.rightIntervals.get(i).rightEndPoint >= q.leftEndPoint)
						retval.add(child.rightIntervals.get(i));
					else if(child.rightIntervals.get(i).leftEndPoint > q.leftEndPoint && child.rightIntervals.get(i).rightEndPoint < q.rightEndPoint)
						retval.add(child.rightIntervals.get(i));
				}
			}
			if(tmp1 && tmp2)
				return retval;
			retval.addAll(IntSect(q, retval, child.leftChild));
			retval.addAll(IntSect(q, retval, child.rightChild));
		}
		return retval;
	}
	private static void sortIntervals(ArrayList<Interval> intervals, char lr){
		if(intervals.isEmpty() || intervals == null){
			return;
		}
		ArrayList<Interval> endpoints = new ArrayList<Interval>();
		int i = 0;
		endpoints.add(intervals.get(0));
		boolean first = true;
		if(lr == 'l'){
			for(Interval e : intervals){
				if(first){
					first = false;
					continue;
				}else{
					if(e.leftEndPoint >= endpoints.get(i).leftEndPoint){
						endpoints.add(e);
						i++;
					}else{// found endpoint is smaller
						int j = i-1;
						while(j > 0 && e.leftEndPoint < endpoints.get(j).leftEndPoint)
							j--;
						if(j < 0)
							j = 0;
						if(e.leftEndPoint < endpoints.get(j).leftEndPoint)
							endpoints.add(j, e);
						else 
							endpoints.add(j+1, e);
						i++;
					}
				}
			}
			intervals.clear();
			intervals.addAll(endpoints);
			return;
		}else if(lr == 'r'){
			for(Interval e : intervals){
				if(first){
					first = false;
					continue;
				}else{
					if(e.rightEndPoint >= endpoints.get(i).rightEndPoint){
						endpoints.add(e);
						i++;
					}else{// found endpoint is smaller
						int j = i-1;
						while(j > 0 && e.rightEndPoint < endpoints.get(j).rightEndPoint)
							j--;
						if(j < 0)
							j = 0;
						if(e.rightEndPoint < endpoints.get(j).rightEndPoint)
							endpoints.add(j, e);
						else 
							endpoints.add(j+1, e);
						i++;
					}
				}
			}
			intervals.clear();
			intervals.addAll(endpoints);
			return;
		}else{
			return;
		}
	}
	
	/**
	 * Returns the root of this interval tree.
	 * 
	 * @return Root of interval tree.
	 */
	public IntervalTreeNode getRoot() {
		return root;
	}
}

