//RP1
//Patrik De Quevedo
//COP3503C
//1/21/2024

import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;

public class dyslectionary {

    public static void main(String[] args) {

        Scanner stdin = new Scanner(System.in);

        while (stdin.hasNext()) { //loop thru diff. groups
        
            ArrayList<String> mylist = new ArrayList<String>();
            String temp = stdin.nextLine();

            //puts words in for each group
            while(!temp.isEmpty()) {
                mylist.add(temp);
                if (stdin.hasNextLine()) {
                    temp = stdin.nextLine();
                } else {
                    temp = "";
                }
            }

            //sorts by last letter
            Collections.sort(mylist, (a, b) -> {
                int minLength = Math.min(a.length(), b.length());

                for (int i = 1; i <= minLength; i++) {

                    char lastA = a.charAt(a.length() - i);
                    char lastB = b.charAt(b.length() - i);

                    if (lastA != lastB) {
                        return lastA - lastB;
                    }
                }
                return a.length() - b.length();
            });

            int maxLen = 0;

            //Find the maximum length of the strings in the ArrayList for spacing 
            for (String s : mylist) {
                maxLen = Math.max(s.length(), maxLen);
            }

            //Prints words with correct spacing
            for (String s : mylist) {
                System.out.println(String.format("%" + maxLen + "s", s));
            }

            // Print a blank line to separate groups
            if (stdin.hasNext()) {
                System.out.println();
            }

        }
        stdin.close();
    }
}
