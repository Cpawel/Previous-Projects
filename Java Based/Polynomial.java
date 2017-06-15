package poly;

import java.io.*;
import java.util.StringTokenizer;

/**
 * This class implements a term of a polynomial.
 * 
 * @author runb-cs112
 *
 */
class Term {
	/**
	 * Coefficient of term.
	 */
	public float coeff;
	
	/**
	 * Degree of term.
	 */
	public int degree;
	
	/**
	 * Initializes an instance with given coefficient and degree.
	 * 
	 * @param coeff Coefficient
	 * @param degree Degree
	 */
	public Term(float coeff, int degree) {
		this.coeff = coeff;
		this.degree = degree;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object other) {
		return other != null &&
		other instanceof Term &&
		coeff == ((Term)other).coeff &&
		degree == ((Term)other).degree;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (degree == 0) {
			return coeff + "";
		} else if (degree == 1) {
			return coeff + "x";
		} else {
			return coeff + "x^" + degree;
		}
	}
}

/**
 * This class implements a linked list node that contains a Term instance.
 * 
 * @author runb-cs112
 *
 */
class Node {
	
	/**
	 * Term instance. 
	 */
	Term term;
	
	/**
	 * Next node in linked list. 
	 */
	Node next;
	
	/**
	 * Initializes this node with a term with given coefficient and degree,
	 * pointing to the given next node.
	 * 
	 * @param coeff Coefficient of term
	 * @param degree Degree of term
	 * @param next Next node
	 */
	public Node(float coeff, int degree, Node next) {
		term = new Term(coeff, degree);
		this.next = next;
	}
}

/**
 * This class implements a polynomial.
 * 
 * @author runb-cs112
 *
 */
public class Polynomial {
	
	/**
	 * Pointer to the front of the linked list that stores the polynomial. 
	 */ 
	Node poly;
	
	/** 
	 * Initializes this polynomial to empty, i.e. there are no terms.
	 *
	 */
	public Polynomial() {
		poly = null;
	}
	
	/**
	 * Reads a polynomial from an input stream (file or keyboard). The storage format
	 * of the polynomial is:
	 * <pre>
	 *     <coeff> <degree>
	 *     <coeff> <degree>
	 *     ...
	 *     <coeff> <degree>
	 * </pre>
	 * with the guarantee that degrees will be in descending order. For example:
	 * <pre>
	 *      4 5
	 *     -2 3
	 *      2 1
	 *      3 0
	 * </pre>
	 * which represents the polynomial:
	 * <pre>
	 *      4*x^5 - 2*x^3 + 2*x + 3 
	 * </pre>
	 * 
	 * @param br BufferedReader from which a polynomial is to be read
	 * @throws IOException If there is any input error in reading the polynomial
	 */
	public Polynomial(BufferedReader br) throws IOException {
		String line;
		StringTokenizer tokenizer;
		float coeff;
		int degree;
		
		poly = null;
		
		while ((line = br.readLine()) != null) {
			tokenizer = new StringTokenizer(line);
			coeff = Float.parseFloat(tokenizer.nextToken());
			degree = Integer.parseInt(tokenizer.nextToken());
			poly = new Node(coeff, degree, poly);
		}
	}
	
	
	/**
	 * Returns the polynomial obtained by adding the given polynomial p
	 * to this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p Polynomial to be added
	 * @return A new polynomial which is the sum of this polynomial and p.
	 */
	
	public Polynomial add(Polynomial p) {
		
		if((p == null || p.poly == null) && (this.poly == null || this == null)){
			return new Polynomial();
		} else if(this.poly == null || this == null){
			return p;
		}else if(p == null || p.poly == null){
			return this;
		}
		Polynomial tmp = new Polynomial();// New polynomial to fill
		Node i = this.poly; // Nodes to progress through the polynomials
		Node j = p.poly;
		boolean first = false; // first node should be skipped
		if(i.term.degree == j.term.degree){ // These allow for the creation of poly.
			if(i.term.coeff + j.term.coeff == 0){
				first = true;
			}
			tmp.poly = new Node(i.term.coeff+j.term.coeff, i.term.degree,null);
			i = i.next;
			j = j.next;
		}else if(i.term.degree > j.term.degree){
			tmp.poly = new Node(j.term.coeff,j.term.degree,null);
			j = j.next;
		}else{
			tmp.poly = new Node(i.term.coeff, i.term.degree, null);
			i = i.next;
		}
		Node pointer = tmp.poly; // Points to the head of the new polynomial as a reference for the end
		while(i != null && j != null){
			if(i.term.degree == j.term.degree){
				if(i.term.coeff + j.term.coeff == 0){
					i = i.next;
					j = j.next;
					continue;
				} else {
					tmp.poly.next = new Node(i.term.coeff+j.term.coeff, i.term.degree,null);
					tmp.poly = tmp.poly.next;
					i = i.next;
					j = j.next;
				}
				
			}else if(i.term.degree > j.term.degree){
				tmp.poly.next = new Node(j.term.coeff,j.term.degree,null);
				tmp.poly = tmp.poly.next;
				j = j.next;
			}else{
				tmp.poly.next = new Node(i.term.coeff, i.term.degree, null);
				tmp.poly = tmp.poly.next;
				i = i.next;
			}
		}
		
		if(i != null && j == null){
			tmp.poly.next = i;
		}else if(j != null && i == null){
			tmp.poly.next = j;
		}
		
		tmp.poly = pointer;
		pointer = null;
		if(first){
			tmp.poly = tmp.poly.next;
		}
		return tmp;
	}
	
	/**
	 * Returns the polynomial obtained by multiplying the given polynomial p
	 * with this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p Polynomial with which this polynomial is to be multiplied
	 * @return A new polynomial which is the product of this polynomial and p.
	 */
	public Polynomial multiply(Polynomial p) {
		
		if(p == null || p.poly == null || this == null || this.poly == null)
			return new Polynomial();
		
		Polynomial tmp = new Polynomial();
		boolean skip = false;
		boolean first = true;
		tmp.poly = new Node(p.poly.term.coeff * this.poly.term.coeff, p.poly.term.degree + this.poly.term.degree, null);
		if(tmp.poly.term.coeff == 0){
			skip = true;
		}
		Node pointer = tmp.poly;
		for(Node i = this.poly; i != null; i = i.next){
			for(Node j = p.poly; j != null; j = j.next){
				if(first){
					j = p.poly.next;
					first = false;
				}
				if(p.poly.term.coeff * this.poly.term.coeff == 0){
					continue;
				}else{
					tmp.poly.next = new Node(j.term.coeff * i.term.coeff, j.term.degree + i.term.degree, null);
					tmp.poly = tmp.poly.next;
				}
			}
		}
		tmp.poly = pointer;
		pointer = null;
		if(skip){
			tmp.poly = tmp.poly.next;
		}
		if(tmp.poly == null || tmp.poly.next == null){
			return tmp;
		}
		
		Polynomial tmp2 = new Polynomial();
		
		Node i = tmp.poly;
		Node j = tmp.poly.next;
		first = true;
		while(i != null){
			while(j != null){
				if(i.term.degree == j.term.degree){
					i.term.coeff += j.term.coeff;
					j.term.coeff = 0;
					j.term.degree = 0;
					j = j.next;
				}else{
					j = j.next;
				}
			}
			if(first){
				tmp2.poly = new Node(i.term.coeff, i.term.degree,null);
				pointer = tmp2.poly;
				first = false;
			}else if(i.term.coeff != 0 && i.term.degree != 0){
				tmp2.poly.next = new Node(i.term.coeff, i.term.degree,null);
				tmp2.poly = tmp2.poly.next;
				if(i.next == null){
					break;
				}
			}
			if(i.next.term.coeff == 0 && i.next.term.degree == 0){
				while(i.next != null && i.next.term.coeff == 0 && i.next.term.degree == 0){
					i = i.next;
					j = i.next;
				}
			}else{
				i=i.next;
				j=i.next;
			}
		}
		tmp2.poly = pointer; // still unsorted
		if(tmp2.poly == null || tmp2.poly.next == null){
			return tmp2;
		}
		
		return tmp2.Sort(); // Sort is a helper method, at the end of this file
	}
	
	/**
	 * Evaluates this polynomial at the given value of x
	 * 
	 * @param x Value at which this polynomial is to be evaluated
	 * @return Value of this polynomial at x
	 */
	public float evaluate(float x) {
		float tmp = 0;
		for(Node i = poly; i != null; i = i.next){
			tmp += (float) (i.term.coeff * Math.pow(x, i.term.degree));
		}
		return tmp;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		String retval;
		
		if (poly == null) {
			return "0";
		} else {
			retval = poly.term.toString();
			for (Node current = poly.next ;
			current != null ;
			current = current.next) {
				retval = current.term.toString() + " + " + retval;
			}
			return retval;
		}
	}
	private Polynomial Sort(){
		Polynomial tmp = new Polynomial();
		
		Node smallest = this.poly;
		Node j = this.poly;
		Node head = this.poly;
		Node pointer = null;
		boolean first = true;
		while(j != null){
			if(j.term.degree < smallest.term.degree && j.term.coeff != 0){
				smallest = j;
				j = j.next;
			}else if(j.term.degree < smallest.term.degree && j.term.coeff == 0){
				j = j.next;
				continue;
			}else if(j.next == null && smallest.term.degree != j.term.degree){
				if(first){
					first = false;
					tmp.poly = new Node(smallest.term.coeff, smallest.term.degree,null);
					pointer = tmp.poly;
					while(this.poly.term.degree != smallest.term.degree){
						this.poly = this.poly.next;
					}
					this.poly.term.coeff = 0;
					this.poly = head;
					j = this.poly;
					smallest.term.degree = 99;
					
				}else{
					tmp.poly.next = new Node(smallest.term.coeff, smallest.term.degree, null);
					tmp.poly = tmp.poly.next;
					while(this.poly.term.degree != smallest.term.degree){
						this.poly = this.poly.next;
					}
					this.poly.term.coeff = 0;
					this.poly = head;
					j = this.poly;
					smallest.term.degree = 99;
				}
			}else if(j.next == null && smallest.term.degree == j.term.degree){
				if(first){
					first = false;
					tmp.poly = new Node(smallest.term.coeff, smallest.term.degree,null);
					return tmp;
				}else{
					tmp.poly.next = new Node(smallest.term.coeff, smallest.term.degree,null);
					tmp.poly = tmp.poly.next;
					break;
				}
			}else{
				j = j.next;
			}
		}
		if(smallest.term.coeff != 0 && smallest.term.degree != 0){
			tmp.poly.next = new Node(smallest.term.coeff, smallest.term.degree,null);
		}
		
		tmp.poly = pointer;
		
		return tmp;
		
	}
}
