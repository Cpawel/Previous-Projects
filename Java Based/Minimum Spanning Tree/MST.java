package apps;

import structures.*;
import java.util.ArrayList;



public class MST {
	
	/**
	 * Initializes the algorithm by building single-vertex partial trees
	 * 
	 * @param graph Graph for which the MST is to be found
	 * @return The initial partial tree list
	 */
	public static PartialTreeList initialize(Graph graph) {
		if(graph == null)
			return null;
		PartialTreeList item = new PartialTreeList();
		PartialTree temp = null;
		for(Vertex e : graph.vertices){
			temp = new PartialTree(e);
			Vertex.Neighbor nbr = e.neighbors;
			while(nbr != null){
				temp.getArcs().insert(new PartialTree.Arc(e, nbr.vertex, nbr.weight));
				nbr = nbr.next;
			}
			item.append(temp);
		}
		return item;
	}

	/**
	 * Executes the algorithm on a graph, starting with the initial partial tree list
	 * 
	 * @param graph Graph for which MST is to be found
	 * @param ptlist Initial partial tree list
	 * @return Array list of all arcs that are in the MST - sequence of arcs is irrelevant
	 */
	public static ArrayList<PartialTree.Arc> execute(Graph graph, PartialTreeList ptlist) {
		if(graph == null || ptlist == null)
			return null;
		PartialTreeList x = ptlist;
		ArrayList<PartialTree.Arc> item = new ArrayList<PartialTree.Arc>();
		while(x.size() > 1){
			PartialTree ptx = x.remove(), pty;
			MinHeap<PartialTree.Arc> pqx = ptx.getArcs();
			PartialTree.Arc arc = (PartialTree.Arc)pqx.deleteMin();
			Vertex v1 = arc.v1, v2 = arc.v2;
			while(v2.getRoot().equals(v1.getRoot())){
				arc = (PartialTree.Arc)pqx.deleteMin();
				v1 = arc.v1;
				v2 = arc.v2;
			}
			item.add(arc);
			pty = x.removeTreeContaining(v2);
			ptx.merge(pty);
			x.append(ptx);
		}
		return item;
	}
}
