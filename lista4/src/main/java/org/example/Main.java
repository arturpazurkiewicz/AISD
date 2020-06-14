package org.example;

import net.sourceforge.argparse4j.ArgumentParsers;
import net.sourceforge.argparse4j.inf.ArgumentParser;
import net.sourceforge.argparse4j.inf.ArgumentParserException;
import net.sourceforge.argparse4j.inf.Namespace;

import java.io.File;
import java.io.FileNotFoundException;
import java.time.Instant;
import java.util.*;

public class Main {

    public static void main(String[] args) {
        ArgumentParser parser = ArgumentParsers.newFor("lista4-1.0.jar").build()
                .description("BST | RBT | HMAP program");
        parser.addArgument("-t", "--type").help("Select type of structure").
                choices("bst", "rbt", "hmap").required(false).setDefault("rbt");
        parser.addArgument("-m").help("Type m for hmap").type(Integer.class).setDefault(53).required(false);
        parser.addArgument("-nt").help("Type nt for hmap").type(Integer.class).setDefault(4).required(false);
        parser.addArgument("-i", "--input").help("Type filename with commands").required(true).type(String.class);
        parser.addArgument("--mode").help("Change working mode (explained in readme)").type(String.class).
                choices("normal", "test1", "test2").setDefault("normal");
        parser.addArgument("--min").help("Argument used in test1 min < nt").setDefault(0).type(Integer.class);
        parser.addArgument("--max").help("Argument used in test1 nt < max").setDefault(5).type(Integer.class);
        parser.addArgument("-k").help("Argument used in test1, test will be multiplied k times").setDefault(1).type(Integer.class);

        try {
            Namespace cli = parser.parseArgs(args);
            String type = cli.get("type");
            int m = cli.get("m");
            int nt = cli.get("nt");
            int k = cli.get("k");
            int min_arg = cli.get("min");
            int max_arg = cli.get("max");
            File file = new File((String) cli.get("input"));
            switch ((String) cli.get("mode")) {
                case "normal":
//                    Normal mode
                    Structure structure = structureSelect(type, m, nt);

                    try {
                        Scanner scanner = new Scanner(file);
                        int i_c = 0, l_c = 0, d_c = 0, f_c = 0, min_c = 0, max_c = 0, s_c = 0, in_c = 0;
                        long i_t = 0, l_t = 0, d_t = 0, f_t = 0, min_t = 0, max_t = 0, s_t = 0, in_t = 0, st = 0, end = 0;
                        st = Instant.now().toEpochMilli();
                        while (scanner.hasNextLine()) {
                            String[] command = scanner.nextLine().split("\\s+");
                            long start = Instant.now().toEpochMilli();
                            switch (command[0]) {
                                case "insert":
                                    structure.insert(command[1]);
                                    i_t += Instant.now().toEpochMilli() - start;
                                    i_c++;
                                    break;
                                case "load":
                                    structure.load(command[1]);
                                    l_t += Instant.now().toEpochMilli() - start;
                                    l_c++;
                                    break;
                                case "delete":
                                    structure.delete(command[1]);
                                    d_t += Instant.now().toEpochMilli() - start;
                                    d_c++;
                                    break;
                                case "find":
                                    System.out.println(structure.find(command[1]));
                                    f_t += Instant.now().toEpochMilli() - start;
                                    f_c++;
                                    break;
                                case "min":
                                    System.out.println(structure.min());
                                    min_t += Instant.now().toEpochMilli() - start;
                                    min_c++;
                                    break;
                                case "max":
                                    System.out.println(structure.max());
                                    max_t += Instant.now().toEpochMilli() - start;
                                    max_c++;
                                    break;
                                case "successor":
                                    System.out.println(structure.successor(command[1]));
                                    s_t += Instant.now().toEpochMilli() - start;
                                    s_c++;
                                case "inorder":
                                    List<String> arrayList = structure.inorder();
                                    if (arrayList.size() > 0)
                                        System.out.println(arrayList.toString());
                                    else {
                                        System.out.println("");
                                    }

                                    in_t += Instant.now().toEpochMilli() - start;
                                    in_c++;
                            }
                        }
                        end = Instant.now().toEpochMilli();
                        long time = end - st;
                        System.err.println("Time(ms): " + time + " Max size: " + structure.max_size + " Current size: " + structure.current_size);
                        try {
                            System.err.println("Insert - invokes: " + i_c + " avg time: " + i_t / i_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("Load - invokes: " + l_c + " avg time: " + l_t / l_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("Delete - invokes: " + d_c + " avg time: " + d_t / d_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("Find - invokes: " + f_c + " avg time: " + f_t / f_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("Min - invokes: " + min_c + " avg time: " + min_t / min_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("Max - invokes: " + max_c + " avg time: " + max_t / max_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("Successor - invokes: " + s_c + " avg time: " + s_t / s_c);
                        } catch (ArithmeticException ignored) {
                        }
                        try {
                            System.err.println("InOrder - invokes: " + in_c + " avg time: " + in_t / in_c);
                        } catch (ArithmeticException ignored) {
                        }
                        scanner.close();
                    } catch (FileNotFoundException e) {
                        System.out.println("File with commands does not exist!");
                        System.out.println(file.getAbsolutePath());
                    }
                    break;
//                    Test mode nr 1 ( exercise 1, for choosing correct nt)
                case "test1":
                    System.out.println("Warning! In this mode, program recognise only load and insert from command file");
                    System.out.println("Program will calculate full time of insertions and full time of finding all keys");
                    try {
                        ArrayList<TimeData> times = new ArrayList<>();
                        for (int nt1 = min_arg + 1; nt1 < max_arg; nt1++) {
                            long findTime = 0;
                            long insertTime = 0;
                            for (int i = 0; i < k; i++) {
                                Scanner scanner = new Scanner(file);
                                HashMap hashMap = new HashMap(m, nt1);
                                hashMap.addWords = true;
                                long st = Instant.now().toEpochMilli();
                                testScanner(hashMap, scanner);
                                insertTime += Instant.now().toEpochMilli() - st;
                                st = Instant.now().toEpochMilli();
                                for (String word : hashMap.allWordsEver) {
                                    if (hashMap.find(word) == 0)
                                        System.out.println("Program does not work correctly");
                                }
                                findTime += Instant.now().toEpochMilli() - st;
                            }
                            System.out.println("Still calculating");
                            times.add(new TimeData(insertTime / k, findTime / k, nt1));
                        }
                        times.sort(new Comparator<TimeData>() {
                            @Override
                            public int compare(TimeData o1, TimeData o2) {
                                int a = (int) ((o1.findTime * 20 + o1.insertionTime) - ((o2.findTime * 20 + o2.insertionTime)));
                                if (a != 0)
                                    return a;
                                return o1.nt - o2.nt;
                            }
                        });
                        System.out.println("\nNT in correct order (higher == better)");
                        for (TimeData timeData : times)
                            System.out.println(timeData);
                    } catch (FileNotFoundException e) {
                        System.out.println("File with commands does not exist!");
                        System.out.println(file.getAbsolutePath());

                    }
                    break;

                case "test2":
//                    Test mode nr 2, exercise 2
                    structure = structureSelect(type, m, nt);
                    try {
                        System.out.println("Warning! In this mode, program recognise only load and insert from command file");
                        System.out.println("Program will calculate minimum, average and maximum comparisons for all finds in provided data");
                        Scanner scanner = new Scanner(file);
                        structure.addWords = true;
                        testScanner(structure, scanner);
                        LinkedList<Integer> compares = new LinkedList<>();
                        for (String word : structure.allWordsEver) {
                            structure.find(word);
                            compares.add(structure.compareCounter);
                        }
                        compares.sort(new Comparator<Integer>() {
                            @Override
                            public int compare(Integer o1, Integer o2) {
                                return o1 - o2;
                            }
                        });
                        try {
                            int min = compares.get(0);
                            int min_c = 0;
                            int max = compares.get(compares.size() - 1);
                            int max_c = 0;
                            for (int x : compares) {
                                if (x == min) {
                                    min_c++;
                                } else if (x == max)
                                    max_c++;
                            }
                            double sum = 0;
                            for (int d : compares) sum += d;
                            double average = sum / compares.size();
                            System.err.println("Current size: " + structure.current_size);
                            System.out.println("Minimum comparisons: " + min + " repeated: " + min_c);
                            System.out.println("Maximum comparisons: " + max + " repeated: " + max_c);
                            System.out.println("Average comparisons: " + average);
                        } catch (IndexOutOfBoundsException e) {
                            System.out.println("No data");
                        }


                    } catch (FileNotFoundException e) {
                        System.out.println("File with commands does not exist!");
                        System.out.println(file.getAbsolutePath());
                    }

                    break;
            }


        } catch (ArgumentParserException e) {
            parser.handleError(e);
        }
    }

    private static void testScanner(Structure structure, Scanner scanner) {
        while (scanner.hasNextLine()) {
            String[] command = scanner.nextLine().split("\\s+");
            switch (command[0]) {
                case "load":
                    structure.load(command[1]);
                case "insert":
                    structure.insert(command[1]);
            }
        }
    }

    private static Structure structureSelect(String type, int m, int nt) {
        Structure structure;
        switch (type) {
            case "bst":
                structure = new BinarySearchTree();
                break;
            case "rbt":
                structure = new RedBlackTree();
                break;
            default:
                structure = new HashMap(nt, m);
        }
        return structure;
    }

    static class TimeData {
        long insertionTime;
        long findTime;
        int nt;

        public TimeData(long insertionTime, long findTime, int nt) {
            this.insertionTime = insertionTime;
            this.findTime = findTime;
            this.nt = nt;
        }

        @Override
        public String toString() {
            return "For nt = " + nt + "\n     insertion time is: " + insertionTime +
                    "\n     find time is: " + findTime;
        }
    }
}
