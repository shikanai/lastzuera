opt-3.5 -S -dce ../minijava/BubbleSort.ll > BubbleSort.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/BubbleSort.ll -o BubbleSort.opt_mine.ll
diff BubbleSort.opt.ll BubbleSort.opt_mine.ll > diff_BubbleSort.txt

opt-3.5 -S -dce ../minijava/LinearSearch.ll > LinearSearch.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/LinearSearch.ll -o LinearSearch.opt_mine.ll
diff LinearSearch.opt.ll LinearSearch.opt_mine.ll > diff_LinearSearch.txt

opt-3.5 -S -dce ../minijava/BinarySearch.ll > BinarySearch.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/BinarySearch.ll -o BinarySearch.opt_mine.ll
diff BinarySearch.opt.ll BinarySearch.opt_mine.ll > diff_BinarySearch.txt

opt-3.5 -S -dce ../minijava/BinaryTree.ll > BinaryTree.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/BinaryTree.ll -o BinaryTree.opt_mine.ll
diff BinaryTree.opt.ll BinaryTree.opt_mine.ll > diff_BinaryTree.txt


opt-3.5 -S -dce ../minijava/Factorial.ll > Factorial.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/Factorial.ll -o Factorial.opt_mine.ll
diff Factorial.opt.ll Factorial.opt_mine.ll > diff_Factorial.txt


opt-3.5 -S -dce ../minijava/LinkedList.ll > LinkedList.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/LinkedList.ll -o LinkedList.opt_mine.ll
diff LinkedList.opt.ll LinkedList.opt_mine.ll > diff_LinkedList.txt


opt-3.5 -S -dce ../minijava/QuickSort.ll > QuickSort.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/QuickSort.ll -o QuickSort.opt_mine.ll
diff QuickSort.opt.ll QuickSort.opt_mine.ll > diff_QuickSort.txt

opt-3.5 -S -dce ../minijava/TreeVisitor.ll > TreeVisitor.opt.ll
opt-3.5 -S -load Release/P3.so -DCE_liveness ../minijava/TreeVisitor.ll -o TreeVisitor.opt_mine.ll
diff TreeVisitor.opt.ll TreeVisitor.opt_mine.ll > diff_TreeVisitor.txt

diff ../minijava/BubbleSort.ll BubbleSort.opt_mine.ll > opt_BubbleSort.txt
diff ../minijava/LinearSearch.ll LinearSearch.opt_mine.ll > opt_LinearSearch.txt
diff ../minijava/BinarySearch.ll BinarySearch.opt_mine.ll > opt_BinarySearch.txt
diff ../minijava/BinaryTree.ll BinaryTree.opt_mine.ll > opt_BinaryTree.txt
diff ../minijava/Factorial.ll Factorial.opt_mine.ll > opt_Factorial.txt
diff ../minijava/LinkedList.ll LinkedList.opt_mine.ll > opt_LinkedList.txt
diff ../minijava/QuickSort.ll QuickSort.opt_mine.ll > opt_QuickSort.txt
diff ../minijava/TreeVisitor.ll TreeVisitor.opt_mine.ll > opt_TreeVisitor.txt

