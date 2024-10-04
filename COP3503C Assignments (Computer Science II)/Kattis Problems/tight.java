// Patrik De Quevedo
// RP5: Kattis  - Tight Words
// 4/10/2024
// COP3503C

import java.util.*;
import java.util.Scanner;

public class tight {

    public static void main(String[] args) {
    	
        Scanner stdin = new Scanner(System.in);

        while (stdin.hasNext()) {
        	
            int k = stdin.nextInt();
            int n = stdin.nextInt();

            if (k <= 1) {
                System.out.println("100.000000000");
                continue;
            }

            double[] curr = new double[k + 1];
            double[] prev = new double[k + 1];
            
            //	Words of length 1
            for (int i = 0; i <= k; i++) {
                prev[i] = 100.0 / (k + 1);
            }

            // Words greater than 1
            for (int j = 2; j <= n; j++) {
                curr[0] = (prev[1] + prev[0]) / (k + 1);
                
                for (int m = 1; m < k; m++) {
                    curr[m] = (prev[m + 1] + prev[m] + prev[m - 1]) / (k + 1);
                }
                
                curr[k] = (+ prev[k] + prev[k - 1]) / (k + 1);

                System.arraycopy(curr, 0, prev, 0, k + 1);
            }

            //	Calculate Total 
            double res = 0.000000000;
            
            for (double num : prev) {
                res += num;
            }

            System.out.printf("%.9f\n", res);
        }//while end

        stdin.close();
    }//main end
    
}//tight end