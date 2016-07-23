/*
 * Maxwell Renke
 * CS 830
 * Assignment 10 - Digit Recognizer
 * Last Edit: November 23, 2015
*/

import java.util.*;
import java.io.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class DigitRecognizer{
	
	/* Arguments */
	public static int attributes;
	public static int values;
	public static int classes;
	
	/* Input */
	public static ArrayList<ArrayList<Integer>> training_data;
	public static ArrayList<ArrayList<Integer>> test_data;
	
	/* knn */
	public static int k = 5; //magic k!
	public static PriorityQueue<ArrayList<Integer>> neighbors;
	public static ArrayList<ArrayList<Integer>> klist;
	public static ArrayList<Integer> kcount;
	
	/* linear */
	public static ArrayList<ArrayList<Float>> theta = new ArrayList<ArrayList<Float>>();
	
	/* nb */
	public static ArrayList<Float> nb_result = new ArrayList<Float>(classes);
	
	/* Return ArrayList<Integer> from String */
	public static ArrayList<Integer> getInts(String s){
		Matcher m = Pattern.compile("\\d+").matcher(s);
        ArrayList<Integer> numbers = new ArrayList<Integer>();
        while(m.find()) {
            numbers.add(Integer.parseInt(m.group()));
        }
		return numbers;
	}
	
	/* Custom comparator for knn max heap */
	public static class kComparator implements Comparator<ArrayList<Integer>>
	{
		public int compare( ArrayList<Integer> a, ArrayList<Integer> b )
		{
			return b.get(1) - a.get(1); //kNN here
		}
	}
	
	/* knn output function */
	public static void knn_output(){
		for( int i=0; i<klist.size(); i++){
			int label = klist.get(i).get(0);
			kcount.set(label, kcount.get(label) + 1); //increment
		}
		
		int c = klist.get(klist.size()-1).get(0); //label
		float conf = (float)kcount.get(c) / (float)k; //confidence
		
		System.out.printf("%d %f\n", c, conf);
	}
	
	/* k-nearest neighbor */
	public static void knn(){
		/* Begin testing */
		for( ArrayList<Integer> test : test_data){
			for( ArrayList<Integer> data : training_data ){
				ArrayList<Integer> dst = new ArrayList<Integer>(data.size());
				dst.add(0,data.get(0)); //class
				
				int d = 0; //calculate distance
				for( int i=0; i<test.size(); i++){
					int delta = data.get(i+1) - test.get(i); //euclidean distance
					d += delta * delta;
				}
				d = (int)Math.sqrt(d);
				dst.add(1,d); //d(a,b)
				
				neighbors.add(dst); //add neighbor
				if( neighbors.size() > k ){ //keep neighbors < k
					neighbors.poll();
				}
			}
			
			for( int i=0; i<k; i++){
				ArrayList<Integer> arr = neighbors.poll();
				if( arr != null ) klist.add(arr);
			}
			
			/* Output */
			knn_output();	
			
			/* Reset */
			for( int i=0; i<kcount.size(); i++){
				kcount.set(i,0);
			}
			klist.clear();
			neighbors.clear();
		}
	}
	
	/* on-line linear regression using LMS */
	public static void linear(){
		/* Get query */
		ArrayList<Integer> query = test_data.get(0);
		
		/* Find match */
		float max = Float.NEGATIVE_INFINITY;
		int out = 0; 
		/* For each classifier */
		for( int c=0; c<classes; c++ ){
			/* Find result */
			float y = 0.0f;
			y += theta.get(c).get(0) * 1.0f;
			for( int i=0; i<attributes; i++ ){
				y += theta.get(c).get(i+1) * query.get(i);
			}
					
			/* Find max */
			if( y > max ){
				max = y;
				out = c;
			}
		}
		
		/* Output */
		float conf = (1.0f+max)/2.0f;
		System.out.println(out + " " + conf);
	}
	
	/* Train Linear */
	public static void train_linear(){
		/* Set alpha */
		float alpha = 0.0001f;
		
		/* Initialization */
		for( int i=0; i<classes; i++ ){
			theta.add(new ArrayList<Float>(attributes));
			for( int j=0; j<attributes+1; j++ ){
				theta.get(i).add(0.0f);
			}
		}
		
		/* Train */
		for( ArrayList<Integer> data : training_data ){
			/* Create new array with x[0] = 1 */
			ArrayList<Float> x = new ArrayList<Float>();
			for( Integer i : data){
				x.add((float)i);
			}
			x.set(0,1.0f);
			
			/* For each classifier */
			for( int c=0; c<classes; c++){
				/* Find y */
				float y = -1.0f;
				if( c == data.get(0) ){
					y = 1.0f;
				}
				
				/* Find y_ (estimate) */
				float y_ = 0.0f;
				for( int i=0; i<attributes+1; i++){
					y_ += theta.get(c).get(i) * x.get(i);
				}
				
				/* Update theta */
				for( int i=0; i<attributes+1; i++){
					float t = theta.get(c).get(i);
					t = t - (alpha * (y_ - y) * x.get(i));
					theta.get(c).set(i,t);
				}
			}
		}
	}
	
	/* naive Bayes */
	public static void nb(){
		/* Get query */
		ArrayList<Integer> query = test_data.get(0);
		
		/* max value */
		float max = Float.NEGATIVE_INFINITY;
		
		/* For each class */
		int H = 0;
		for( int c=0; c<classes; c++){
			float p = 1.0f;
			float count = 0.0f; //blank
			float prob = 0.0f; //blank
			for( int i=0; i<attributes; i++){
				int x = query.get(i); //get attribute i from query
				count = probHx.get(c).get(x).get(i); //count for class c, get value x at position i
				prob = ((count)) / (countH.get(c));  //P(x(i)|H) / countH
				p *= prob; //sum product
			}
			
			p *= countH.get(c)/((float)training_data.size()+1.0f);
			//System.err.println("p: " + p);
			if( p > max && countH.get(c) > 1.0f){
				max = p;
				H = c;
			}
		}
		
		/* Output */
		System.out.println(H + " " + max);
	}
	
	public static ArrayList<ArrayList<ArrayList<Float>>> probHx = new ArrayList<ArrayList<ArrayList<Float>>>();
	public static ArrayList<Float> countH = new ArrayList<Float>();
	
	/* Train naive Bayes */
	public static void train_nb(){
		/* Set alpha */
		float alpha = 0.01f;
		
		/* Initialization */
		for( int i=0; i<classes; i++){
			countH.add(1.0f); //initialize count of each H to be 1
			probHx.add(new ArrayList<ArrayList<Float>>());
			for( int j=0; j<values; j++){
				probHx.get(i).add(new ArrayList<Float>());
				for( int k=0; k<attributes; k++){
					probHx.get(i).get(j).add(1.0f); //initialize each value at each attribute to 1
				}
			}
		}
		
		for( ArrayList<Integer> data : training_data){
			int H = data.get(0);
			countH.set(H,countH.get(H)+1.0f);
			for( int i=0; i<attributes; i++){
				int x = data.get(i+1);
				float inc = probHx.get(H).get(x).get(i);
				inc = inc + 1.0f; //increment
				probHx.get(H).get(x).set(i, inc);
			}
		}
		
		/* debug prints */
		/*int count = 0;
		System.out.println(countH);
		for( ArrayList<ArrayList<Float>> Hx : probHx ){
			System.out.println(count++);
			for( ArrayList<Float> values : Hx ){
				System.out.println(values);
			}
			System.out.println("");
		}*/
	}
	
	/* Abstraction of algorithm choice */
	public static void perform(String alg){
		if( alg.equals("knn") ){
			knn();
		} else if( alg.equals("linear") ){		
			linear();
		} else if( alg.equals("nb") ){
			nb();
		}
	}
	
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		
		/* Get algorithm */
		String alg = args[0];
		
		/* Read in parameters */
		String buffer = sc.nextLine();
		
        ArrayList<Integer> nums = getInts(buffer);
		
		attributes = nums.get(0);
		values = nums.get(1);
		classes = nums.get(2);
		/* End Parameters */
		
		training_data = new ArrayList<ArrayList<Integer>>();
		test_data = new ArrayList<ArrayList<Integer>>();
		
		neighbors = new PriorityQueue<ArrayList<Integer>>(k, new kComparator());
		klist = new ArrayList<ArrayList<Integer>>(k);
		kcount = new ArrayList<Integer>(classes);
		for( int i=0; i<classes; i++){
			kcount.add(i,0); //initialize
		}

		String training = sc.nextLine(); //dummy
		
		/* Read in training data */
		buffer = sc.nextLine();
		while( !buffer.contains("-- test --") ){
			nums = getInts(buffer);
			training_data.add(nums);
			buffer = sc.nextLine();
		}

		/* Train linear models if applicable */
		if( alg.equals("linear") ){
			train_linear();
		}
		
		/* Train nb model if applicable */
		if( alg.equals("nb") ){
			train_nb();
		}
		
		/* Read in test data */
		buffer = sc.nextLine();
		while( buffer.length() > 0 ){
			test_data.add(getInts(buffer));
			
			/* Execute algorithm */
			perform(alg);
			
			test_data.clear();
			
			if( sc.hasNextLine() ){
				buffer = sc.nextLine();
			} else {
				break;
			}
		}
		/* Main end */
	}
}
