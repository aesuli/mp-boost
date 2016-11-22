MP-Boost++ is a C++ implementation of AdaBoost.MH and MPBoost, a multi-label-specific variant of AdaBoost that is more accurate and requires less iterations.

Andrea Esuli
http://www.esuli.it

Scientific results produced using the software provided shall acknowledge the use of MP-Boost++. Please cite as:

Andrea Esuli, Tiziano Fagni, and Fabrizio Sebastiani
MP-Boost: A Multiple-Pivot Boosting Algorithm and its Application to Text Categorization
Proceedings of the 13th International Symposium on String Processing and Information Retrieval (SPIRE’06),
Glasgow, UK, 2006, pages 1-12. Lecture Notes in Computer Science n. 4209, Springer Verlag.

http://www.esuli.it/wp-content/uploads/2011/07/SPIRE06a.pdf

###Compiling MP-Boost++

~~~~
~/mp-boost/> tar zxvf mpboost_latest_source.tar.gz
~/mp-boost/> make
~~~~

That should be enough, provided that you have make and g++ (currently using version 4.3.3).
Binaries are created directly into the mpboost directory.

~~~~
-rwxrwxr-x 1 esuli esuli 91156 2010-08-16 16:09 boostTest
-rwxrwxr-x 1 esuli esuli 86596 2010-08-16 16:09 boostTrain
-rwxrwxr-x 1 esuli esuli 25216 2010-08-16 16:09 mergeEvaluation
-rwxrwxr-x 1 esuli esuli 40025 2010-08-16 16:09 showEvaluation
~~~~

###Input format

MPBoost and AdaBoost.MH use binary features, so no weight information is provided for the features.
MPBoost and AdaBoost.MH are multi-label classification algorithms, i.e., a document could belong to zero, one, or more than one category.

The input format for MP-Boost++ is based on a sparse vector representation, using text files and describing one vector per line.
The format of a line describing a vector is:

~~~~
<ID> <featureID>* | <categoryID>*
~~~~

with feature IDs and category IDs sorted in ascending order. The

For example, the line describing document with ID 3, containing features 3, 6, 103, and 201, and belonging to categories 3 and 9 is:

~~~~
3 3 6 103 201 | 3 9
~~~~

The pipe character is used to define where the feature list ends and category list starts. If a document has no category, the pipe at the end of the line can be omitted.

###Reuters21578 data

[Download the Reuters21578 collection in MPBoost++ format here.](http://www.esuli.it/data/reuters21578.tar.bz2)

###Train and Test

In order to perform a train and test experiment, MPBoost++ must be provided with a training data file and one (or more) test data file.
The boostTrain program is used to learn a classification model out of the training data file.
The boostTest program is used to classify test data using the learned classification model.
The showEvaluation program is used to show the evaluation of the classification results, in the form of contingency tables and effectiveness measures. The output of showEvaluation is optimized for direct copy & paste into a spreadsheet.
In the case multiple test data files are used (e.g., for RCV1 v2) the mergeEvaluation program can be used to merge the partial evaluation on each test data file into a single evaluation.


~~~~
~/mpboost/> cd data
~/mpboost/data/> bunzip2 reuters21578.tar.bz2
~/mpboost/data/> tar xvf reuters21578.tar
~/mpboost/data/> cd ..
~/mpboost/> ./boostTrain -t ./data/reuters21578/training

Loading data
Using uniform distribution
Data loaded
Starting training
Iteration 1
...
Iteration 100
Training completed in 111.86 seconds.
Serializing model to file: ./data/reuters21578/training.model
Serialization completed
Serializing distribution to file: ./data/reuters21578/training.distribution
Serialization completed

~/mpboost/> ./boostTest -t ./data/reuters21578/test -m ./data/reuters21578/training.model

Loading data
Using all the hypothesis in the model (100)
Data loaded
Starting test
Test completed in 9.04 seconds.
Serializing evaluation to file: ./data/reuters21578/test.evaluation
Serialization completed
Serializing prediction to file: ./data/reuters21578/test.prediction
Serialization completed

~/mpboost/> ./showEvaluation -e ./data/reuters21578/test.evaluation

Evaluation file: ./data/reuters21578/test.evaluation
Global table
TP TN FN FP
2984 375235 760 406
Per-category tables
cat TP TN FN FP
0 619 2553 100 27
114 0.999394 0.866667 1 0.928571
MACRO-average evaluation
accuracy precision recall F1
0.996927 0.479644 0.410518 0.427168
MICRO-average evaluation
accuracy precision recall F1
0.996927 0.880236 0.797009 0.836557
~~~~
