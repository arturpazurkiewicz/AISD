package org.example;


import java.util.ArrayList;
import java.util.List;

public class BinarySearchTree extends Structure {
    BinaryTreeNode root;
    static class BinaryTreeNode {
        public BinaryTreeNode left, right;
        public String key;

        public BinaryTreeNode(String elem) {
            key = elem;
            left = right = null;
        }
        private StringBuilder toString(StringBuilder prefix, boolean isLeft, StringBuilder sb) {
            if (right != null) {
                right.toString(new StringBuilder().append(prefix).append(isLeft ? "│   " : "    "), false, sb);
            }
            sb.append(prefix).append(isLeft ? "└── " : "┌── ").append(key).append("\n");
            if (left != null) {
                left.toString(new StringBuilder().append(prefix).append(isLeft ? "    " : "│   "), true, sb);
            }
            return sb;
        }

        @Override
        public String toString() {
            System.out.println();
            return this.toString(new StringBuilder().append("   "), true, new StringBuilder()).toString();
        }
    }

    @Override
    public void ins(String key) {
//        root = insertRec(root, key, 1);
        BinaryTreeNode newElem = new BinaryTreeNode(key);
        BinaryTreeNode y = root,x = null;
        while (y != null){
            x = y;
            if (key.compareTo(y.key) < 0)
                y = y.left;
            else {
                y = y.right;
            }
        }
        if (x == null)
            root = newElem;
        else if (key.compareTo(x.key) < 0)
            x.left = newElem;
        else
            x.right = newElem;
        increaseSizeCounter();
    }


    @Override
    public void delete(String key) {
        try {
            root = del(root, key);
            current_size--;
        } catch (NullPointerException ignored) {
        }
    }

    private BinaryTreeNode del(BinaryTreeNode root, String key) {
        if (root != null) {
            if (key.compareTo(root.key) < 0) {
                root.left = del(root.left, key);
            } else if (key.compareTo(root.key) > 0) {
                root.right = del(root.right, key);
            } else {
                if (root.left == null)
                    return root.right;
                else if (root.right == null)
                    return  root.left;
                root.key = minValue(root.right);
                root.right = del( root.right, root.key);
            }
        }
        return root;
    }

    public int find(String element) {
        compareCounter = 0;
        BinaryTreeNode y = root;
        try {
            while (true){
                compareCounter++;
                if (y.key.equals(element))
                    return 1;
                compareCounter++;
                if (y.key.compareTo(element) > 0){
                    y = y.left;
                } else {
                    y = y.right;
                }
            }
        } catch (NullPointerException e){
            return 0;
        }

    }

    public String min() {
        try {
            return minValue(root);
        } catch (NullPointerException e) {
            return "";
        }
    }

    public String max() {
        try {
            return maxValue(root);
        } catch (NullPointerException e) {
            return "";
        }
    }


    public List<String> inorder() {
        return inorderRec(root);
    }
    @Override
    public String successor(String element) {
        try {
            BinaryTreeNode x = findR(root, element);
            if (x.right != null)
                return minValue(x.right);
            else {
                BinaryTreeNode candidate = null;
                BinaryTreeNode y = root;
                while (y != x) {
                    if (x.key.compareTo(y.key) < 0) {
                        candidate = y;
                        y = y.left;
                    } else {
                        y = y.right;
                    }
                }
                return candidate.key;
            }
        } catch (NullPointerException e) {
            return "";
        }
    }


    protected static String minValue(BinaryTreeNode root) {
        String minv = root.key;
        while (root.left != null) {
            minv = root.left.key;
            root = root.left;
        }
        return minv;
    }

    protected static String maxValue(BinaryTreeNode root) {
        String maxv = root.key;
        while (root.right != null) {
            maxv = root.right.key;
            root = root.right;
        }
        return maxv;
    }

    protected List<String> inorderRec(BinaryTreeNode root) {
        if (root != null) {
            List<String> r = new ArrayList<>(inorderRec(root.left));
            r.add(root.key);
            r.addAll(inorderRec(root.right));
            return r;
        } else {
            return new ArrayList<>();
        }
    }

    private BinaryTreeNode findR(BinaryTreeNode root, String element) {
        int a = root.key.compareTo(element);
        compareCounter++;
        if (a > 0)
            return findR(root.left, element);
        else if (a < 0) {
            compareCounter++;
            return findR(root.right, element);
        }
        else {
            compareCounter++;
            return root;
        }
    }

    @Override
    public String toString() {
        if (root == null)
            return "";
        return root.toString();
    }

}
