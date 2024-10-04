//Patrik De Quevedo
//COP3503C - Spring 2024
//P2
//1/28/2024
/*Summary: 
Given 12 distinct numbers, in how many ways, disregarding rotations and
reflections, can you assign the numbers to the vertices such that the sum of the
numbers along each of 6 straight lines passing through 4 vertices is the same?*/


import java.util.*;

public class hexagram {

  public static void main(String[] args) {
  //long startTime = System.nanoTime();
      Scanner stdin = new Scanner(System.in);
     
      int[] puzzle = new int[12];
      int NumSum = 0;

      // Assign values to the puzzle array
      for (int i = 0; i < 12; i++) {
          puzzle[i] = stdin.nextInt();
          NumSum = NumSum + puzzle[i]; // Keep track of total
      }
      int desiredLineSum = NumSum / 3;

      while (zero(puzzle)==false) {
          // Declare other variables here.
          // Read input for one case.
    	  int count = 0;
          count = permute(puzzle, 0, desiredLineSum,count);
          
     
          // Output answer for one case.
          System.out.println(count/12);// div 12 to account for rotations and reflections

          // Takes in the next 12 ints for the next array
          for (int i = 0; i < 12; i++) puzzle[i] = stdin.nextInt();
      }
      stdin.close();

  }// main

  // Checks if all values are zero; if they are, end
  public static boolean zero(int[] puzzle){
  	
  	for (int i=0; i<12; i++) {
  	if (puzzle[i] != 0) return false;
  	}
  	
  	return true;
  }//zero

  //permutation code
  public static int permute(int[] puzzle, int index, int desiredLineSum,int count) {
  	
      if (index == 12) {
      	
        int line1Sum = puzzle[1] + puzzle[2] + puzzle[3] + puzzle[4];
        int line2Sum = puzzle[0] + puzzle[2] + puzzle[5] + puzzle[7];
        int line3Sum = puzzle[7] + puzzle[8] + puzzle[9] + puzzle[10];
        int line4Sum = puzzle[0] + puzzle[3] + puzzle[6] + puzzle[10];
        int line5Sum = puzzle[1] + puzzle[5] + puzzle[8] + puzzle[11];
        int line6Sum = puzzle[4] + puzzle[6] + puzzle[9] + puzzle[11];

        if (line1Sum == desiredLineSum && line4Sum == desiredLineSum &&
            line5Sum == desiredLineSum && line2Sum == desiredLineSum &&
            line3Sum == desiredLineSum && line6Sum == desiredLineSum) {
            
            return count++;
        } 
        else {
      	  return count;
        }
      }//if
      else {	
      	
	        for (int i = index; i < 12; i++) {
	        	
	        	if (index==i) continue;//to not swap with itself
	
	            //Check if the partial sums are valid before making the recursive call
	            int line1Sum = puzzle[1] + puzzle[2] + puzzle[3] + puzzle[4];
	            int line2Sum = puzzle[0] + puzzle[2] + puzzle[5] + puzzle[7];
	            int line3Sum = puzzle[7] + puzzle[8] + puzzle[9] + puzzle[10];
	            int line4Sum = puzzle[0] + puzzle[3] + puzzle[6] + puzzle[10];
	            //int line5Sum = puzzle[1] + puzzle[5] + puzzle[8] + puzzle[11];
	            //int line6Sum = puzzle[4] + puzzle[6] + puzzle[9] + puzzle[11];
	         
	            //stops permutation early
	            
	            //check when line 1 complete
	            if ((index==5) && (line1Sum != desiredLineSum)){	             
	              return count;
	            }    
	      
	            //check when line 2 complete   
	            if ((index==8) && (line2Sum != desiredLineSum)){
	              return count;
	            }
	          
	            //check when line 3 & 4 complete     
	            if ((index==11) && (line3Sum != desiredLineSum || line4Sum != desiredLineSum)){
	              return count;
	            }

	            swap(puzzle, index, i); //swapping the values
	            
	            count += permute(puzzle, index + 1, desiredLineSum,count);
	           
	            swap(puzzle, index, i); //swaps back to original
	        }//for
	        
	        return count;
      }//else
      
      
  }// Perm

  // Swaps elements at indices i and j in the array
  public static void swap(int[] arr, int i, int j) {
      int temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
  }// swap
}// Main (class)