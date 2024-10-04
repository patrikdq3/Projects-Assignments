// Patrik De Quevedo
// RP3: Kattis  - Classroom
// 2/28/2024
// COP3503C

import java.io.*;
import java.util.*;

class Interval implements Comparable<Interval>{
    int s;
    int e;

    Interval(int s, int e) {
        this.s = s;
        this.e = e;
    }
    
    // Sort intervals based on ending time
    @Override
    public int compareTo(Interval o) {
        return this.e - o.e;
    }
}

class Pair implements Comparable<Pair>{
    int first;
    int second;

    Pair(int f, int s) {
        this.first = f;
        this.second = s;
    }
    
    // Sort pairs based on the first element, if first elements are equal, sort based on the second element
    @Override
    public int compareTo(Pair o) {
        if (this.first == o.first) {
            return this.second - o.second;
        }
        return this.first - o.first;
    }
}

public class classrooms {
    public static void main(String[] args) {
        Scanner stdin = new Scanner(System.in);
        
        int n = stdin.nextInt();    //  # of Activities
        int k = stdin.nextInt();    //  # of Classrooms
        
        // TreeSet to maintain available classrooms
        TreeSet<Pair> room = new TreeSet<Pair>();
        
        // List to store intervals
        List<Interval> times = new ArrayList<Interval>();
        
        // Getting input and storing intervals
        while (n > 0){ //Deincrements N bc when n==0 thats when the input lines end
            times.add(new Interval(stdin.nextInt(), stdin.nextInt()));
            n--;
        }
        Collections.sort(times);
        
        // Initialize classrooms
        for (int i = 0; i < k; i++)
            room.add(new Pair(0, i));

        int res = 0; // Counter for max # of activities
        
        // Find an available classroom for the current interval
        for (Interval interval : times) {
            Pair available = room.floor(new Pair(interval.s, k));
            if (available != null) {
                int y = available.second;
                room.remove(available); // Remove the current available classroom
                room.add(new Pair(interval.e + 1, y)); // Update available time for the classroom
                res++;  // Increment activity counter

            }
        }
       stdin.close();
       System.out.println(res);
    }//main
}//classrooms class