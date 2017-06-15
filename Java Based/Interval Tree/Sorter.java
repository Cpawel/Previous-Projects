package structures;

import java.util.ArrayList;

/**
 * This class is a repository of sorting methods used by the interval tree.
 * It's a utility class - all methods are static, and the class cannot be instantiated
 * i.e. no objects can be created for this class.
 * 
 * @author runb-cs112
 */
public class Sorter {
	
	private Sorter() { }
	
	/**
	 * Sorts a set of intervals in place, according to left or right endpoints.  
	 * At the end of the method, the parameter array list is a sorted list. 
	 * 
	 * @param intervals Array list of intervals to be sorted.
	 * @param lr If 'l', then sort is on left endpoints; if 'r', sort is on right endpoints
	 */
	public static void sortIntervals(ArrayList<Interval> intervals, char lr){
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
	 * Given a set of intervals (left sorted and right sorted), extracts the left and right end points,
	 * and returns a sorted list of the combined end points without duplicates.
	 * 
	 * @param leftSortedIntervals Array list of intervals sorted according to left endpoints
	 * @param rightSortedIntervals Array list of intervals sorted according to right endpoints
	 * @return Sorted array list of all endpoints without duplicates
	 * @throws Exception 
	 */
	public static ArrayList<Integer> getSortedEndPoints(ArrayList<Interval> leftSortedIntervals, ArrayList<Interval> rightSortedIntervals) throws Exception {
		if(leftSortedIntervals == null && rightSortedIntervals == null){
			throw new Exception("Both lists are missing!");
		}
		ArrayList<Integer> left = new ArrayList<Integer>();
		ArrayList<Integer> right = new ArrayList<Integer>();
		ArrayList<Integer> fin = new ArrayList<Integer>();
		int i = 0;
		if(!leftSortedIntervals.isEmpty()){
			for(Interval e : leftSortedIntervals){
				if(i == 0)
					left.add(e.leftEndPoint);
				else
					if(left.get(i) == e.leftEndPoint)
						continue;
					else{
						left.add(e.leftEndPoint);
						i++;
					}
			}
		}
		i = 0;
		if(!rightSortedIntervals.isEmpty()){
			for(Interval e : rightSortedIntervals){
				if(i == 0)
					right.add(e.rightEndPoint);
				else
					if(right.get(i) == e.rightEndPoint)
						continue;
					else{
						right.add(e.rightEndPoint);
						i++;
					}
			}
		}
		i = 0;
		int j = 0;
		while(j < right.size() && i < left.size()){
			if(left.get(i) < right.get(j)){
				if(fin.lastIndexOf(left.get(i)) == -1){
					fin.add(left.get(i));
				}
				i++;
			}else if(left.get(i) > right.get(j)){
				if(fin.lastIndexOf(right.get(j)) == -1){
					fin.add(right.get(j));
				}
				j++;
			}else{
				if(fin.lastIndexOf(left.get(i)) == -1){
					fin.add(left.get(i));
				}
				i++;
				j++;
			}
		}
		while(i < left.size()){
			if(fin.lastIndexOf(left.get(i)) == -1)
				fin.add(left.get(i));
			i++;
		}
		while(j < right.size()){
			if(fin.lastIndexOf(right.get(j)) == -1)
				fin.add(right.get(j));
			j++;
		}
		return fin;
	}
}
