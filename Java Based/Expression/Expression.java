package apps;

import java.io.IOException;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.StringTokenizer;

import structures.Stack;

@SuppressWarnings("unused")
public class Expression {
	

	/**
	 * Expression to be evaluated
	 */
	String expr;                
    
	/**
	 * Scalar symbols in the expression 
	 */
	ArrayList<ScalarSymbol> scalars;   
	
	/**
	 * Array symbols in the expression
	 */
	ArrayList<ArraySymbol> arrays;
    
	/**
	 * Positions of opening brackets
	 */
	ArrayList<Integer> openingBracketIndex; 
    
	/**
	 * Positions of closing brackets
	 */
	ArrayList<Integer> closingBracketIndex; 

    /**
     * String containing all delimiters (characters other than variables and constants), 
     * to be used with StringTokenizer
     */
    public static final String delims = " \t*+-/()[]";
    
    /**
     * Initializes this Expression object with an input expression. Sets all other
     * fields to null.
     * 
     * @param expr Expression
     */
    public Expression(String expr) {
        this.expr = expr;
        scalars = null;
        arrays = null;
        openingBracketIndex = null;
        closingBracketIndex = null;
    }

    /**
     * Matches parentheses and square brackets. Populates the openingBracketIndex and
     * closingBracketIndex array lists in such a way that closingBracketIndex[i] is
     * the position of the bracket in the expression that closes an opening bracket
     * at position openingBracketIndex[i]. For example, if the expression is:
     * <pre>
     *    (a+(b-c))*(d+A[4])
     * </pre>
     * then the method would return true, and the array lists would be set to:
     * <pre>
     *    openingBracketIndex: [0 3 10 14]
     *    closingBracketIndex: [8 7 17 16]
     * </pe>
     * 
     * See the FAQ in project description for more details.
     * 
     * @return True if brackets are matched correctly, false if not
     */
    public boolean isLegallyMatched() {
    	openingBracketIndex = new ArrayList<Integer>();
    	closingBracketIndex = new ArrayList<Integer>();
        Stack<Bracket> opens = new Stack<Bracket>();// stores the opening brackets (the type of bracket and its index)
        String expr = this.expr;
        if(expr.isEmpty())// in case nothing is entered
			return true;
        int i = 0;//index
        char a = '(' , b = '[' , c = ')' , d = ']';//easy storage of nescessary components
        if(expr.indexOf(a) == -1 && expr.indexOf(b) == -1 && expr.indexOf(c) == -1 && expr.indexOf(d) == -1)// in case nothing important is entered
        	return true;// is there a way to reduce the above?
        boolean first = true;// used to check if the first bracket found is closing or not
        
        while(i < expr.length()){
        	if(first){
        		if(expr.charAt(i) == c || expr.charAt(i) == d)// closing brackets found first
        			return false;
        	}
        	if(expr.charAt(i) == a || expr.charAt(i) == b){
        		opens.push(new Bracket(expr.charAt(i), i));//pushing in the open brackets
        		first = false;
        	}else if(expr.charAt(i) == c){
        		if(opens.peek().ch == a){
        			openingBracketIndex.add(opens.pop().pos);//pop on finding the respective bracket
        			closingBracketIndex.add(i);
        		}else{
        			return false;//wrong bracket found, ie (]
        		}
        	}else if(expr.charAt(i) == d){
        		if(opens.peek().ch == b){
        			openingBracketIndex.add(opens.pop().pos);
        			closingBracketIndex.add(i);
        		}else{
        			return false;
        		}
        	}
        	i++;
        }
        
        return opens.isEmpty();//if anything is left, uneven number of brackets
    }

    /**
     * Populates the scalars and arrays lists with symbols for scalar and array
     * variables in the expression. For every variable, a SINGLE symbol is created and stored,
     * even if it appears more than once in the expression.
     * At this time, the constructors for ScalarSymbol and ArraySymbol
     * will initialize values to zero and null, respectively.
     * The actual values will be loaded from a file in the loadSymbolValues method.
     */
    public void buildSymbols() {
    	scalars = new ArrayList<ScalarSymbol>();
    	arrays = new ArrayList<ArraySymbol>();
    	String expr = this.expr;
    	StringTokenizer arraysplit = new StringTokenizer(expr, "[");// to find all the array variables first
		ArrayList<String> arrayvars = new ArrayList<String>() , arraysfinal = new ArrayList<String>();
		
		while(arraysplit.hasMoreTokens())
			arrayvars.add(arraysplit.nextToken());// takes in all the splits
		
		for(int i = arrayvars.size() - 2; i >= 0; i--){
			arraysplit = new StringTokenizer(arrayvars.get(i), delims);
			
			while(arraysplit.countTokens() > 1)// pops out tokens until the very last one, where the array variable is
					arraysplit.nextToken();
			
			arraysfinal.add(arraysplit.nextToken()); // holds ALL array vars, including dupes.
			
		}
		
		if(!arraysfinal.isEmpty()){	// in case there are no arrays
			
			arrays.add(new ArraySymbol(arraysfinal.get(0)));
			for(int i = 1; i < arraysfinal.size(); i++){// This loop gets rid of any duplicates by only entering unique vars into arrays
				ArraySymbol tmp = new ArraySymbol(arraysfinal.get(i));
				boolean dupe = false;
				for(int j = 0; j < arrays.size(); j++){
					if(arrays.get(j).equals(tmp)){
						dupe = true;
						break;
					}
				}
				if(!dupe){
					arrays.add(tmp);
				}
			}// this concludes all array variables.
		}
		ArrayList<String> scalarvars = new ArrayList<String>(), scalarsfinal = new ArrayList<String>();
		arraysplit = new StringTokenizer(expr, delims);//reusing this, as it has the same function as before, except for scalars
		while(arraysplit.hasMoreTokens()){
			scalarvars.add(arraysplit.nextToken());//hold ALL variables
		}
		for(String tmp1 : scalarvars){//gets rid of array variables
			boolean dupe = false;
			for(String tmp2 : arraysfinal){
				if(tmp1.equals(tmp2)){
					dupe = true;
					break;
				}
			}
			if(!dupe){
				char rawr = tmp1.charAt(0);// gets rid of actual numbers, but not variables with numbers in them
				if(rawr >= '0' && rawr <= '9'){
					continue;
				}else{
					scalarsfinal.add(tmp1);
				}
			}
		}
		if(!scalarsfinal.isEmpty()){// in the case of no scalar vars
			scalars.add(new ScalarSymbol(scalarsfinal.get(0)));
			for(int i = 1; i < scalarsfinal.size(); i++){// populates scalars with uniques
				ScalarSymbol tmp = new ScalarSymbol(scalarsfinal.get(i));
				boolean dupe = false;
				for(int j = 0; j < scalars.size(); j++){
					if(scalars.get(j).equals(tmp)){
						dupe = true;
						break;
					}
				}
				if(!dupe){
					scalars.add(tmp);
				}
			}
		}// Both lists should be populated with unique variables.
    }
    
    /**
     * Loads values for symbols in the expression
     * 
     * @param sc Scanner for values input
     * @throws IOException If there is a problem with the input 
     */
    public void loadSymbolValues(Scanner sc) 
    throws IOException {
        while (sc.hasNextLine()) {
            StringTokenizer st = new StringTokenizer(sc.nextLine().trim());
            int numTokens = st.countTokens();
            String sym = st.nextToken();
            ScalarSymbol ssymbol = new ScalarSymbol(sym);
            ArraySymbol asymbol = new ArraySymbol(sym);
            int ssi = scalars.indexOf(ssymbol);
            int asi = arrays.indexOf(asymbol);
            if (ssi == -1 && asi == -1) {
            	continue;
            }
            int num = Integer.parseInt(st.nextToken());
            if (numTokens == 2) { // scalar symbol
                scalars.get(ssi).value = num;
            } else { // array symbol
            	asymbol = arrays.get(asi);
            	asymbol.values = new int[num];
                // following are (index,val) pairs
                while (st.hasMoreTokens()) {
                    String tok = st.nextToken();
                    StringTokenizer stt = new StringTokenizer(tok," (,)");
                    int index = Integer.parseInt(stt.nextToken());
                    int val = Integer.parseInt(stt.nextToken());
                    asymbol.values[index] = val;              
                }
            }
        }
    }
    
    /**
     * Evaluates the expression, using RECURSION to evaluate subexpressions and to evaluate array 
     * subscript expressions. (Note: you can use one or more private helper methods
     * to implement the recursion.)
     * 
     * @return Result of evaluation
     * @throws IOException 
     */
    public float evaluate() throws IOException {
    	String expr = this.expr;
    	if(expr.isEmpty())// self explanitory
    		return 0;
    	if(!this.scalars.isEmpty()){// if scalars present, replace them with numbers
    		expr = this.replacer();
    		this.expr = expr; // have to replace the exprs in the case that comes next, as all of these methods only work on Expressions
    	}
    	StringBuffer tmp = new StringBuffer(expr);
    	for(int i = 0; i < tmp.length(); i++){//gets rid of spaces
    		if(' ' == tmp.charAt(i)){
    			tmp.deleteCharAt(i);
    			i--;
    		}
    	}
		return evaluNums(tmp.toString());
    }
    private String replacer(){// replaces all scalar variables with their integer counterpart
    	String expr = this.expr;
    	Expression tmp2 = new Expression(expr);
    	tmp2.scalars = this.scalars;// need to do this in order for all the brackets to be updated
    	int loc = 0, length = 0, i = 0;//location, length of variable, index of variable
    	Integer value;
    	StringBuffer tmp = new StringBuffer(tmp2.expr);//used to easily replace the nescessary parts
    	String[] scalars2 = new String[tmp2.scalars.size()];
    	for(i = 0; i < scalars2.length; i++){
    		scalars2[i] = tmp2.scalars.get(i).name;
    	}
    	boolean swap = true;
        int j = 0;
        while (swap) {
            swap = false;
            j++;
            for (i = 0; i < scalars2.length - j; i++) {
            	
                if (scalars2[i].length() < scalars2[i + 1].length()) {
                    String tmp5 = scalars2[i];
                    scalars2[i] = scalars2[i + 1];
                    scalars2[i + 1] = tmp5;
                    swap = true;
                }

            }

        }
    	
    	ArrayList<ScalarSymbol> scalars3 = new ArrayList<ScalarSymbol>();
    	j = 0; i = 0;
    	int k = 0;
    	while(j < scalars2.length){
    		String temp8 = scalars2[j];
    		String temp9 = scalars.get(i).name;
    		if(temp8.equals(temp9)){
    			ScalarSymbol temp7 = new ScalarSymbol(temp8);
    			scalars3.add(temp7);
    			scalars3.get(k).value = scalars.get(i).value;
    			k++;
    			
    			j++;
    			i = 0;
    		}else{
    			i++;
    		}
    		
    	}
    	i = 0;
    	while( i < scalars3.size()){
    		loc = tmp.indexOf(scalars3.get(i).name);
    		length = scalars3.get(i).name.length();
    		value = scalars3.get(i).value;
    		String value2 = value.toString();
    		int rawr = loc + length;
    		tmp.replace(loc, rawr, value2);//loc + length in order to cover the entire variable, in case it is long
    		tmp2.expr = tmp.toString();
    		
    		if(!tmp2.expr.contains(scalars3.get(i).name)){// check that no other letters are next to said variable
    			i++;
    		}
    	}
    	
    	return tmp2.expr.toString();
    }
    
    private String insider(String expr) throws IOException{// handles the insides of arrays and parens.
    	Expression expr2 = new Expression(expr);
    	expr2.isLegallyMatched(); // populates indexes
    	expr2.arrays = arrays;
    	StringBuffer tmp = new StringBuffer(expr);
    	Integer value;
    	Float value2;
    	if(tmp.indexOf("[") != -1){
    		int loc = 0, length = 0, i = 0, open, close;
    		String arr = expr2.arrays.get(0).name;
    		while(!expr.contains(arr)){
    			i++;
    			arr = expr2.arrays.get(i).name;
    		}// arr now is the name of the array
    		loc = expr.indexOf(arr);
    		length = arr.length();
    		open = loc+length;
    		for(close = 0; open != expr2.openingBracketIndex.get(close); close++);
    		close = expr2.closingBracketIndex.get(close);
    		StringBuffer tmp2 = new StringBuffer(expr.substring(open + 1, close));
    		value = (int) evaluNums(tmp2.toString());
    		value = expr2.arrays.get(i).values[value.intValue()];
    		tmp.replace(loc, close+1, value.toString());
    	}else // no arrays
    	if(tmp.indexOf("(") != -1){
    		int open = tmp.indexOf("(");
    		int close;
    		for(close = 0; open != expr2.openingBracketIndex.get(close); close++);
    		close = expr2.closingBracketIndex.get(close);
    		StringBuffer tmp2 = new StringBuffer(expr.substring(open + 1, close));
    		value2 = evaluNums(tmp2.toString());
    		tmp.replace(open, close+1, value2.toString());
    	}
    	
    	
		return tmp.toString();
    }
    private float evaluNums(String nums) throws IOException{//recursive numerical evaluation. Notice that the order of operations are reversed. This is due to stacks and recursion.
    	while(nums.contains("[") || nums.contains("(")){
    		nums = insider(nums);
    	}
        if (!nums.contains("+") && !nums.contains("-") && !nums.contains("*") && !nums.contains("/")) {
            return Float.parseFloat(nums);
        }// Above, if this instance of evaluate if given only a number, it casts the number from string to float
        
        String left,right;// The left and right sides around an operator
        int i = nums.length() - 1; // Starting the search for operators from the right - so i can reach -1 to indicate not found
        int k, j = nums.indexOf('(');
        while(i >= 0) {
            if (nums.charAt(i) == '+' || nums.charAt(i) == '-')
                break;
            i--;
        }
        if(i != -1){// + or - found
        	left = nums.substring(0, i);
            right = nums.substring(i + 1, nums.length());
        	if(nums.charAt(i) == '+'){
        		return evaluNums(left) + evaluNums(right);
        	}else if(nums.charAt(i) == '-'){
        		return evaluNums(left) - evaluNums(right);
        	}
        }else{// + and - not found
        	i = nums.length() - 1;
            while (i >= 0) {
                if (nums.charAt(i) == '*' || nums.charAt(i) == '/') {
                    break;
                }
                i--;
            }
        }
        left = nums.substring(0, i);
        right = nums.substring(i + 1, nums.length());
        if(nums.charAt(i) == '*'){
        	return evaluNums(left) * evaluNums(right);
        }else if(nums.charAt(i) == '/'){
        	if(evaluNums(right) == '0'){
        		throw new IOException("Cannot divide by zero!");
        	}
        	 return evaluNums(left) / evaluNums(right);
        }
        
        throw new IOException("No idea what happened, should have never reached this.");
    	
    }

    /**
     * Utility method, prints the symbols in the scalars list
     */
    public void printScalars() {
        for (ScalarSymbol ss: scalars) {
            System.out.println(ss);
        }
    }
    
    /**
     * Utility method, prints the symbols in the arrays list
     */
    public void printArrays() {
    	for (ArraySymbol as: arrays) {
    		System.out.println(as);
    	}
    }

}
