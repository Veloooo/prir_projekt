package com.company;

import java.io.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

class Connection {
    int node1;
    int node2;

    public Connection(int node1, int node2) {
        this.node1 = node1;
        this.node2 = node2;
    }

    public int getNode1() {
        return node1;
    }

    public int getNode2() {
        return node2;
    }
}



public class Main {
    public static void whenWriteStringUsingBufferedWritter_thenCorrect(ArrayList<Connection> connections)
            throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter("newFile2.csv"));
        for(Connection c: connections)
            writer.write(c.node1+";"+c.node2 + ";"+ (int)(Math.random()*50)+" \n");
        writer.close();
    }
    public static void main(String[] args) {
        BufferedReader reader;
        ArrayList<Connection> connections = new ArrayList<>();
        HashMap<String, Integer> mappings = new HashMap<>();
        int count = 0;
        try {
            reader = new BufferedReader(new FileReader(
                    "email-Eu-core.txt"));
            String line = reader.readLine();
            while (line != null) {
                String[] split = line.split(" ");
                for (int i = 0; i < 2; i++) {
                    if (!mappings.containsKey(split[i])) {
                        mappings.put(split[i], count);
                        count++;
                    }
                }
                connections.add(new Connection(mappings.get(split[0]), mappings.get(split[1])));
                line = reader.readLine();
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        connections.sort(Comparator.comparingInt(Connection::getNode1).thenComparingInt(Connection::getNode2));
        try {
            whenWriteStringUsingBufferedWritter_thenCorrect(connections);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
