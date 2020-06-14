package org.example;

import org.apache.commons.io.IOUtils;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public abstract class Structure {
    public abstract void ins(String element);

    public int current_size = 0;
    public int max_size = 0;
    public LinkedList<String> allWordsEver = new LinkedList<>();
    public boolean addWords = false;
    public int compareCounter = 0;

    public abstract void delete(String element);


    public void load(String fileName) {
        File myFile = new File(fileName);
        try (InputStream in = new FileInputStream(myFile)) {
            String contents = IOUtils.toString(in, StandardCharsets.UTF_8);
//            System.out.println(contents);
            Matcher m = Pattern.compile("[^\\s]+").matcher(contents);
            while (m.find()) {
                this.insert(m.group());
            }
        } catch (FileNotFoundException e) {
            System.out.println(myFile.getAbsolutePath());
            System.out.println("File " + fileName + " does not exist!");
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public void insert(String element) {
        Matcher m = Pattern.compile("[a-zA-Z].*$").matcher(element);
        while (m.find()) {
            Matcher g = Pattern.compile("^.*[a-zA-Z]").matcher(m.group());
            while (g.find()) {
                if (addWords) {
//                    if (!allWordsEver.contains(g.group()))
                    allWordsEver.add(g.group());
                }

                ins(g.group());
            }
        }
    }

    public void increaseSizeCounter() {
        current_size++;
        if (current_size > max_size)
            max_size = current_size;
    }

    public abstract int find(String element);

    public abstract String min();

    public abstract String max();

    public abstract String successor(String element);

    public abstract List<String> inorder();

}
