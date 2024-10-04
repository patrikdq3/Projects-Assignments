//	Patrik De Quevedo
//	COP3503C
//	P4 - Drones
//	3/17/2024

import java.util.*;

public class drones {
	
    static final int SIZE = 8; // Size of the grid (8x8)

    static int[] Dx = {-1, 0, 0, 1}; 
    static int[] Dy = {0, -1, 1, 0};

    //	Checks if the given coordinates are within the bounds of the grid & returns true if inbounds
    static boolean inbounds(int x, int y) {
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE; 
    }

    //	Checks if the  coordinates are a no fly zone & returns true if its a no fly zone
    static boolean isNoFlyZone(int[][] grid, int x, int y) {
        return grid[x][y] == -1; 
    }
    

    //	Calculates the hash code representing the state of drones on the grid.
    static int hashCode(int[] drones) {
    	
        int hash = 0; 
        
        // Iterate over drone positions & calculate hash code
        for (int i = 0; i < drones.length; i++) { 
            hash |= drones[i] << (6 * i); 
        }
        return hash;
    }

    //	BFS
    static int bfs(int[][] grid, int[] start, int[] end, int n, int[] distance) {
    	
        Queue<Integer> q = new ArrayDeque<>(); 
        
        int[] drones = new int[n]; // Array to store current drone positions
        
        // Initialize drone positions
        for (int i = 0; i < start.length; i++) { 
            drones[i] = start[i];
        }
        
        int hash = hashCode(drones); // Calculate hash code for initial drone positions
        
        q.offer(hash);
        distance[hash] = 0; // distance of initial state set to 0

        
        while (!q.isEmpty()) {
        	
            int curr = q.poll(); 
            
            int[] currDrones = new int[n]; 
            
            // Get drone positions from hash code
            for (int i = 0; i < n; i++) { 
                currDrones[i] = (curr >> (6 * i)) & 63;
            }

            for (int i = 0; i < n; i++) { // Iterate over drones
            	
                if (currDrones[i] == end[i]) continue; 
                
                // Calculate coordinates of current drone
                int x = currDrones[i] / SIZE; 
                int y = currDrones[i] % SIZE; 

                for (int j = 0; j < 4; j++) { // Iterate over possible directions & calculate new coordinates
                	
                    int nx = x + Dx[j]; 
                    int ny = y + Dy[j]; 

                    if (inbounds(nx, ny) && !isNoFlyZone(grid, nx, ny)) { // Check if move is valid
                    	
                        int[] nextDrones = currDrones.clone(); 
                        
                        nextDrones[i] = nx * SIZE + ny; // Update position of current drone
                        
                        int nextHash = hashCode(nextDrones); // Calculate hash code for next state
                        
                        
                        // Check if next state is visited & adds next state to the queue if not visited
                        if (distance[nextHash] == -1) { 
                            distance[nextHash] = distance[curr] + 1;
                            q.offer(nextHash);
                        }
                    }
                }
            }
        }

        int result = Integer.MAX_VALUE;
        
        // Iterate over drones & update result with minimum distance to destination
        for (int i = 0; i < n; i++) { 
            result = Math.min(result, distance[end[i]]); 
        }
        
        return result == Integer.MAX_VALUE ? -1 : result; // Return result or -1 if no valid path exists
    }

    public static void main(String[] args) {
    	
        Scanner stdin = new Scanner(System.in);
        int n = Integer.parseInt(stdin.nextLine()); // # of drones

        int[][] grid = new int[SIZE][SIZE]; // Grid representation to keep track of no fly zones
        int[] start = new int[n]; // Starting positions of drones
        int[] end = new int[n]; // Destinations of drones
        int[] distance = new int[1 << (6 * n)]; // Array to store distances / Min length

        // Initialize distance array, -1 = unvisited
        for (int i = 0; i < distance.length; i++) {
            distance[i] = -1;
        }

        // Takes in user input to fill end, start, and grid arrays
        for (int i = 0; i < SIZE; i++) {
        	
            String line = stdin.nextLine();
            String[] tokens = line.split(" ");
            
            for (int j = 0; j < SIZE; j++) {
                if (tokens[j].startsWith("G")) {
                    end[Integer.parseInt(tokens[j].substring(1)) - 1] = i * SIZE + j;
                    
                } else if (tokens[j].startsWith("D")) {
                    start[Integer.parseInt(tokens[j].substring(1)) - 1] = i * SIZE + j;
                    
                } else if (tokens[j].equals("XX")) {
                    grid[i][j] = -1;
                }
            }
        }

        // Runs bfs & outputs shortest path for drones to get to destinations
        System.out.println(bfs(grid, start, end, n, distance));
        stdin.close();
    }
}
