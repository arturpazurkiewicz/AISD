package org.example;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class HashMap extends Structure {
    private final int nt;
    private final int m;
    private HashBucket[] hashBuckets;

    public HashMap(int nt, int m) {
        this.nt = nt;
        this.m = m;
        hashBuckets = new HashBucket[m];
        for (int i = 0; i < m; i++) {
            hashBuckets[i] = new HashBucket(nt);
        }
    }

    static class HashBucket {
        Object data;
        private final int nt;
        private int actualSize = 0;

        public HashBucket(int nt) {
            data = new LinkedList<String>();
            this.nt = nt;
        }

        public boolean insert(String s) {
            if (data instanceof LinkedList && actualSize < nt) {
                LinkedList<String> a = (LinkedList<String>) data;
//                if (!a.contains(s)) {
//                    a.add(s);
//                    actualSize++;
//                    return true;
//                }
                a.add(s);
                actualSize++;
                return true;
            } else if (data instanceof LinkedList && actualSize == nt) {
                LinkedList<String> a = (LinkedList<String>) data;
                RedBlackTree n = new RedBlackTree();
                for (int i = 0; i < actualSize; i++) {
                    n.ins(a.get(i));
                }
                data = n;
            }
            RedBlackTree n = (RedBlackTree) data;
            int z = n.current_size;
            n.ins(s);
            if (n.current_size > z)
                actualSize++;

            return n.current_size > z;
        }

        public boolean delete(String s) {
            if (data instanceof RedBlackTree) {
                RedBlackTree n = (RedBlackTree) data;
                int size = n.current_size;
                n.delete(s);
                if (n.current_size < size)
                    actualSize--;
                return size > n.current_size;
            }
            LinkedList<String> a = (LinkedList<String>) data;
            int l = a.size();
            a.remove(s);
            if (a.size() < l)
                actualSize--;
            return l > a.size();
        }

        public int find(String s) {
            if (data instanceof RedBlackTree) {
                if (((RedBlackTree) data).find(s) == 0)
                    return -1 * ((RedBlackTree) data).compareCounter;
                return ((RedBlackTree) data).compareCounter;
            }
            LinkedList<String> a = (LinkedList<String>) data;
            for (int i = 0; i < actualSize; i++) {
                if (a.get(i).equals(s))
                    return i + 1;
            }
            return -1 * actualSize;
        }

        public List<String> words() {
            if (data instanceof RedBlackTree)
                return ((RedBlackTree) data).inorder();
            return (LinkedList<String>) data;
        }
    }

    private int hashFunction(String s, int m) {
        int x = s.charAt(0);
        int a;
        if (64 < x && x < 91)
            a = x - 64;
        else if (96 < x && x < 123)
            a = x - 70;
        else
            a = 0;
        return a % m;
    }

    @Override
    public void ins(String s) {
        int b = hashFunction(s, m);
        if (hashBuckets[b].insert(s))
            increaseSizeCounter();
        else {
            System.out.println(s);
        }
    }

    @Override
    public void delete(String element) {
        int b = hashFunction(element, m);
        if (hashBuckets[b].delete(element))
            current_size--;
    }

    @Override
    public int find(String element) {
        int b = hashFunction(element, m);
        int a = hashBuckets[b].find(element);
        if (a > 0) {
            compareCounter = a;
            return 1;
        }
        compareCounter = -1 * a;
        return 0;
    }


    @Override
    public String min() {
        return "";
    }

    @Override
    public String max() {
        return "";
    }

    @Override
    public String successor(String element) {
        return "";
    }

    @Override
    public List<String> inorder() {
        return new ArrayList<>();
    }
}
