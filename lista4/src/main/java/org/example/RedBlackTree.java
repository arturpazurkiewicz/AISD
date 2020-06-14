package org.example;


import java.util.ArrayList;
import java.util.List;

public class RedBlackTree extends Structure {
    static class RedBlackNode {
        String key;
        RedBlackNode parent;
        RedBlackNode left;
        RedBlackNode right;
        int color;
    }


    private RedBlackNode root;
    private RedBlackNode TNULL;


    private List<String> inOrderHelper(RedBlackNode node) {
        if (node != TNULL) {
            List<String> r = new ArrayList<>(inOrderHelper(node.left));
            r.add(node.key);
            r.addAll(inOrderHelper(node.right));
            return r;
        }
        return new ArrayList<>();
    }

    private RedBlackNode searchTreeHelper(RedBlackNode node, String key) {
        if (node == TNULL || key.equals(node.key)) {
            return node;
        }

        if (key.compareTo(node.key) < 0) {
            return searchTreeHelper(node.left, key);
        }
        return searchTreeHelper(node.right, key);
    }

    private void fixDelete(RedBlackNode x) {
        RedBlackNode s;
        while (x != root && x.color == 0) {
            if (x == x.parent.left) {
                s = x.parent.right;
                if (s.color == 1) {
                    s.color = 0;
                    x.parent.color = 1;
                    leftRotate(x.parent);
                    s = x.parent.right;
                }

                if (s.left.color == 0 && s.right.color == 0) {
                    s.color = 1;
                    x = x.parent;
                } else {
                    if (s.right.color == 0) {
                        s.left.color = 0;
                        s.color = 1;
                        rightRotate(s);
                        s = x.parent.right;
                    }

                    s.color = x.parent.color;
                    x.parent.color = 0;
                    s.right.color = 0;
                    leftRotate(x.parent);
                    x = root;
                }
            } else {
                s = x.parent.left;
                if (s.color == 1) {
                    s.color = 0;
                    x.parent.color = 1;
                    rightRotate(x.parent);
                    s = x.parent.left;
                }
                if (s.right.color == 0 && s.left.color == 0) {
                    s.color = 1;
                    x = x.parent;
                } else {
                    if (s.left.color == 0) {
                        s.right.color = 0;
                        s.color = 1;
                        leftRotate(s);
                        s = x.parent.left;
                    }

                    s.color = x.parent.color;
                    x.parent.color = 0;
                    s.left.color = 0;
                    rightRotate(x.parent);
                    x = root;
                }
            }
        }
        x.color = 0;
    }

    private void rbTransplant(RedBlackNode u, RedBlackNode v) {
        if (u.parent == null) {
            root = v;
        } else if (u == u.parent.left) {
            u.parent.left = v;
        } else {
            u.parent.right = v;
        }
        v.parent = u.parent;
    }

    private void deleteNodeHelper(RedBlackNode node, String key) {
        RedBlackNode z = TNULL;
        RedBlackNode x, y;
        while (node != TNULL) {
            if (node.key.equals(key)) {
                z = node;
            }

            if (node.key.compareTo(key) <= 0) {
                node = node.right;
            } else {
                node = node.left;
            }
        }

        if (z == TNULL) {
            return;
        }
        current_size--;
        y = z;
        int yOriginalColor = y.color;
        if (z.left == TNULL) {
            x = z.right;
            rbTransplant(z, z.right);
        } else if (z.right == TNULL) {
            x = z.left;
            rbTransplant(z, z.left);
        } else {
            y = minimum(z.right);
            yOriginalColor = y.color;
            x = y.right;
            if (y.parent == z) {
                x.parent = y;
            } else {
                rbTransplant(y, y.right);
                y.right = z.right;
                y.right.parent = y;
            }

            rbTransplant(z, y);
            y.left = z.left;
            y.left.parent = y;
            y.color = z.color;
        }
        if (yOriginalColor == 0) {
            fixDelete(x);
        }
    }

    private void fixInsert(RedBlackNode k) {
        RedBlackNode u;
        while (k.parent.color == 1) {
            if (k.parent == k.parent.parent.right) {
                u = k.parent.parent.left;
                if (u.color == 1) {
                    u.color = 0;
                    k.parent.color = 0;
                    k.parent.parent.color = 1;
                    k = k.parent.parent;
                } else {
                    if (k == k.parent.left) {
                        k = k.parent;
                        rightRotate(k);
                    }
                    k.parent.color = 0;
                    k.parent.parent.color = 1;
                    leftRotate(k.parent.parent);
                }
            } else {
                u = k.parent.parent.right;

                if (u.color == 1) {
                    u.color = 0;
                    k.parent.color = 0;
                    k.parent.parent.color = 1;
                    k = k.parent.parent;
                } else {
                    if (k == k.parent.right) {
                        k = k.parent;
                        leftRotate(k);
                    }
                    k.parent.color = 0;
                    k.parent.parent.color = 1;
                    rightRotate(k.parent.parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root.color = 0;
    }

    private void printHelper(RedBlackNode root, String indent, boolean last) {
        if (root != TNULL) {
            System.out.print(indent);
            if (last) {
                System.out.print("R----");
                indent += "   ";
            } else {
                System.out.print("L----");
                indent += "|  ";
            }

            String sColor = root.color == 1 ? "RED" : "BLACK";
            System.out.println(root.key + "(" + sColor + ")");
            printHelper(root.left, indent, false);
            printHelper(root.right, indent, true);
        }
    }

    public RedBlackTree() {
        TNULL = new RedBlackNode();
        TNULL.color = 0;
        TNULL.left = null;
        TNULL.right = null;
        root = TNULL;
    }

    @Override
    public List<String> inorder() {
        return inOrderHelper(this.root);

    }


    public RedBlackNode searchTree(String k) {
        return searchTreeHelper(this.root, k);
    }

    @Override
    public String min() {
        RedBlackNode r = minimum(root);
        if (r == TNULL)
            return "";
        return r.key;
    }

    @Override
    public String max() {
        try {
            RedBlackNode r = maximum(root);
            return r.key;
        } catch (NullPointerException e){
            return "";
        }

    }


    public RedBlackNode minimum(RedBlackNode node) {
        while (node.left != TNULL) {
            node = node.left;
        }
        return node;
    }

    public RedBlackNode maximum(RedBlackNode node) {
        while (node.right != TNULL) {
            node = node.right;
        }
        return node;
    }

    private RedBlackNode findR(RedBlackNode root, String element) {
        if (root == TNULL)
            throw new NullPointerException();
        int a = root.key.compareTo(element);
        compareCounter++;
        if (a > 0)
            return findR(root.left, element);
        else if (a < 0)
            return findR(root.right, element);
        else {
            return root;
        }
    }

    @Override
    public String successor(String element) {
        try {
           RedBlackNode x = findR(root, element);
            if (x.right != null)
                return minimum(x.right).key;
            else {
                RedBlackNode candidate = null;
                RedBlackNode y = root;
                while (y != x) {
                    if (y == TNULL)
                        break;
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

    public void leftRotate(RedBlackNode x) {
        RedBlackNode y = x.right;
        x.right = y.left;
        if (y.left != TNULL) {
            y.left.parent = x;
        }
        y.parent = x.parent;
        if (x.parent == null) {
            this.root = y;
        } else if (x == x.parent.left) {
            x.parent.left = y;
        } else {
            x.parent.right = y;
        }
        y.left = x;
        x.parent = y;
    }

    public void rightRotate(RedBlackNode x) {
        RedBlackNode y = x.left;
        x.left = y.right;
        if (y.right != TNULL) {
            y.right.parent = x;
        }
        y.parent = x.parent;
        if (x.parent == null) {
            this.root = y;
        } else if (x == x.parent.right) {
            x.parent.right = y;
        } else {
            x.parent.left = y;
        }
        y.right = x;
        x.parent = y;
    }

    @Override
    public void ins(String key) {
        RedBlackNode node = new RedBlackNode();
        node.parent = null;
        node.key = key;
        node.left = TNULL;
        node.right = TNULL;
        node.color = 1;

        RedBlackNode y = null;
        RedBlackNode x = this.root;

        while (x != TNULL) {
            y = x;
            if (node.key.compareTo(x.key) < 0) {
                x = x.left;
            } else {
                x = x.right;
            }
        }
        increaseSizeCounter();
        node.parent = y;
        if (y == null) {
            root = node;
        } else if (node.key.compareTo(y.key) < 0) {
            y.left = node;
        } else {
            y.right = node;
        }

        if (node.parent == null) {
            node.color = 0;
            return;
        }

        if (node.parent.parent == null) {
            return;
        }

        fixInsert(node);
    }



    @Override
    public void delete(String data) {
        deleteNodeHelper(this.root, data);
    }

    @Override
    public int find(String element) {
        compareCounter = 0;
        RedBlackNode y = root;
        try {
            while (y != TNULL){
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
            return 0;
        } catch (NullPointerException e){
            return 0;
        }

    }

    public void printTree() {
        printHelper(this.root, "", true);
    }
}

