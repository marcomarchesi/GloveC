//
//  GestureRecognizer.cpp
//  GloveRuntime
//
//  Created by Marco Marchesi on 3/17/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#include "GestureRecognizer.h"

#define TRIM_THRESHOLD 0.01
#define TRIM_PERCENTAGE 90

#define GESTURE_THRESHOLD 0.6
#define FINGER_THRESHOLD 0.5

using namespace std;

/* GENERATE A RANDOM TRAINING SET */
TimeSeriesClassificationData GestureRecognizer::generate_random_set(int dimension,int classes){
    //Create a new instance of the TimeSeriesClassificationData
    TimeSeriesClassificationData trainingData;
    
        //Set the dimensionality of the data (you need to do this before you can add any samples)
        trainingData.setNumDimensions(dimension );
    
        //You can also give the dataset a name (the name should have no spaces)
        trainingData.setDatasetName("GloveData");
    
        //You can also add some info text about the data
        trainingData.setInfoText("This data contains gesture data");
    
        //Here you would record a time series, when you have finished recording the time series then add the training sample to the training data
        UINT gestureLabel = 1;
        MatrixDouble trainingSample;
    
        //For now we will just add 10 x 20 random walk data timeseries
        Random random;
        for(UINT k=0; k<classes; k++){//For the number of classes
            gestureLabel = k+1;
    
            //Get the init random walk position for this gesture
            VectorDouble startPos( trainingData.getNumDimensions() );
            for(UINT j=0; j<startPos.size(); j++){
                startPos[j] = random.getRandomNumberUniform(-1.0,1.0);
            }
    
            //Generate the 20 time series
            for(UINT x=0; x<20; x++){
    
                //Clear any previous timeseries
                trainingSample.clear();
    
                //Generate the random walk
                UINT randomWalkLength = random.getRandomNumberInt(90, 110);
                VectorDouble sample = startPos;
                for(UINT i=0; i<randomWalkLength; i++){
                    for(UINT j=0; j<startPos.size(); j++){
                        sample[j] += random.getRandomNumberUniform(-0.1,0.1);
                    }
    
                    //Add the sample to the training sample
                    trainingSample.push_back( sample );
                }
                //Add the training sample to the dataset
                trainingData.addSample( gestureLabel, trainingSample );
            }
        }
    
        //After recording your training data you can then save it to a file
        if( !trainingData.saveDatasetToFile( "TrainingData.txt" ) ){
            cout << "Failed to save dataset to file!\n";
            return EXIT_FAILURE;
        }
    
    return EXIT_SUCCESS;
};

/* GET SOME INFO */
int GestureRecognizer::info(){
    
    LabelledTimeSeriesClassificationData trainingData;
    //This can then be loaded later
    if( !trainingData.loadDatasetFromFile( "TrainingData.grt" ) ){
        cout << "Failed to load dataset from file!\n";
        return EXIT_FAILURE;
    }
    
    //This is how you can get some stats from the training data
    string datasetName = trainingData.getDatasetName();
    string infoText = trainingData.getInfoText();
    UINT numSamples = trainingData.getNumSamples();
    UINT numDimensions = trainingData.getNumDimensions();
    UINT numClasses = trainingData.getNumClasses();
    
    cout << "Dataset Name: " << datasetName << endl;
    cout << "InfoText: " << infoText << endl;
    cout << "NumberOfSamples: " << numSamples << endl;
    cout << "NumberOfDimensions: " << numDimensions << endl;
    cout << "NumberOfClasses: " << numClasses << endl;
    
    //You can also get the minimum and maximum ranges of the data
    vector< MinMax > ranges = trainingData.getRanges();
    
    cout << "The ranges of the dataset are: \n";
    for(UINT j=0; j<ranges.size(); j++){
        cout << "Dimension: " << j << " Min: " << ranges[j].minValue << " Max: " << ranges[j].maxValue << endl;
    }

    return EXIT_SUCCESS;
};

/* GESTURRE RECOGNITION PIPELINE INITIALIZATION */
int GestureRecognizer::init(){
    
    dtw.enableNullRejection( true ); // to solve GESTURE SPOTTING
//    dtw.setNullRejectionCoeff(1);
    
    // TODO check if correct
//    dtw.enableZNormalization(true); //Nick Gillian says it's not working well with accelerometer data
//    dtw.setOffsetTimeseriesUsingFirstSample(true);
    
    //Load the DTW model from a file
    if( !pipeline.loadPipelineFromFile("DTWPipeline.grt") ){
        cout << "Failed to load the classifier model!\n";
        return EXIT_FAILURE;
    }
    
    return 0;
}

/* INITIALIZE THE RECOGNIZER */
int GestureRecognizer::train(){
    
    LabelledTimeSeriesClassificationData trainingData;
    
    
    /* load training data */
    if( !trainingData.loadDatasetFromFile( "TrainingData.grt" ) ){
        cout << "Failed to load dataset from file!\n";
        return EXIT_FAILURE;
    }
    
    // I COMMENTED TRIMMING, IT LOOKS LIKE WITHOUT WORKS BETTER
    //Trim the training data for any sections of non-movement at the start or end of the recordings
    dtw.enableTrimTrainingData(true,TRIM_THRESHOLD,TRIM_PERCENTAGE);
    
    cout << "Training Data loaded" << endl;
//    trainingData.printStats();
    
    
    
    //If you want to partition the dataset into a training dataset and a test dataset then you can use the partition function
    //A value of 80 means that 80% of the original data will remain in the training dataset and 20% will be returned as the test dataset
    LabelledTimeSeriesClassificationData testData = trainingData.partition( 80 );
    
    //If you have multiple datasets that you want to merge together then use the merge function
    if( !trainingData.merge( testData ) ){
        cout << "Failed to merge datasets!\n";
        return EXIT_FAILURE;
    }
    
    //If you want to run K-Fold cross validation using the dataset then you should first spilt the dataset into K-Folds
    //A value of 10 splits the dataset into 10 folds and the true parameter signals that stratified sampling should be used
    if( !trainingData.spiltDataIntoKFolds( 10, true ) ){
        cout << "Failed to spiltDataIntoKFolds!\n";
        return EXIT_FAILURE;
    }
    
    //After you have called the spilt function you can then get the training and test sets for each fold
    for(UINT foldIndex=0; foldIndex<10; foldIndex++){
        TimeSeriesClassificationData foldTrainingData = trainingData.getTrainingFoldData( foldIndex );
        TimeSeriesClassificationData foldTestingData = trainingData.getTestFoldData( foldIndex );
    }
    
    //pipeline can be used only if a classifier has been set
    pipeline.setClassifier(dtw);
    
    //Train the classifier
    if( !pipeline.train( trainingData ) ){
        cout << "Failed to train classifier!\n";
        return EXIT_FAILURE;
    }
    
    //Save the DTW model to a file
    if( !pipeline.savePipelineToFile("DTWPipeline.grt") ){
        cout << "Failed to save the classifier model!\n";
        return EXIT_FAILURE;
    }
    
    
    
//    //Test the pipeline using the test data
//    cout << "Testing model..." << endl;
//    if( !pipeline.test( testData ) ){
//        cout << "ERROR: Failed to test the pipeline!\n";
//        return EXIT_FAILURE;
//    }
//    
//    //Print some stats about the testing
//    cout << "Test Accuracy: " << pipeline.getTestAccuracy() << endl;
//    
//    cout << "Precision: ";
//    for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
//        UINT classLabel = pipeline.getClassLabels()[k];
//        cout << "\t" << pipeline.getTestPrecision(classLabel);
//    }cout << endl;
//    
//    cout << "Recall: ";
//    for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
//        UINT classLabel = pipeline.getClassLabels()[k];
//        cout << "\t" << pipeline.getTestRecall(classLabel);
//    }cout << endl;
//    
//    cout << "FMeasure: ";
//    for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
//        UINT classLabel = pipeline.getClassLabels()[k];
//        cout << "\t" << pipeline.getTestFMeasure(classLabel);
//    }cout << endl;
//    
//    MatrixDouble confusionMatrix = pipeline.getTestConfusionMatrix();
//    cout << "ConfusionMatrix: \n";
//    for(UINT i=0; i<confusionMatrix.getNumRows(); i++){
//        for(UINT j=0; j<confusionMatrix.getNumCols(); j++){
//            cout << confusionMatrix[i][j] << "\t";
//        }cout << endl;
//    }
    
//    //Get the current null rejection thresholds from the classifier
//    VectorDouble thresholds = pipeline.getNullRejectionThresholds();
//    cout << thresholds[0] << endl;
//    cout << thresholds[1] << endl;
//    thresholds[1] = 77;
//    dtw.setNullRejectionThresholds(thresholds);
    //Update the thresholds
    
    //Use the test dataset to test the DTW model
    double accuracy = 0;
    for(GRT::UINT i=0; i<testData.getNumSamples(); i++){
        //Get the i'th test sample - this is a timeseries
        GRT::UINT classLabel = testData[i].getClassLabel();
        GRT::MatrixDouble local_timeseries = testData[i].getData();
        
        //Perform a prediction using the classifier
        if( !pipeline.predict( local_timeseries ) ){
            cout << "Failed to perform prediction for test sample: " << i <<"\n";
            return EXIT_FAILURE;
        }
        
        //Get the predicted class label
        GRT::UINT predictedClassLabel = pipeline.getPredictedClassLabel();
        double maximumLikelihood = pipeline.getMaximumLikelihood();
        GRT::VectorDouble classLikelihoods = pipeline.getClassLikelihoods();
        GRT::VectorDouble classDistances = pipeline.getClassDistances();
        
        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
        
        cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << endl;
    }
    
    cout << "Test Accuracy: " << accuracy/double(testData.getNumSamples())*100.0 << "%" << endl;
    
     trainingData.clear();

    return EXIT_SUCCESS;
};

/* classify the data acquired from the glove controller */

int GestureRecognizer::classify(VectorDouble gloveDataMatrix){
    
    

    
//    GRT::MatrixDouble timeseries = gloveData[0].getData();
    //Perform a prediction using the classifier
    if( !pipeline.predict( gloveDataMatrix ) ){
        cout << "Failed to perform prediction for glove sample.\n";
        return EXIT_FAILURE;
    }
    //Get the predicted class label --> read dtw for get it
    GRT::UINT predictedClassLabel = pipeline.getPredictedClassLabel();
    GRT::VectorDouble classDistances = pipeline.getClassDistances();
    GRT::VectorDouble classLikelihoods = pipeline.getClassLikelihoods();
    double maximumLikelihood = pipeline.getMaximumLikelihood();
    if(predictedClassLabel != 0){
        switch (predictedClassLabel) {
            case 1:
                cout << "MIC" << endl;
                break;
            case 2:
                cout << "SQUARE" <<endl;
                break;
            case 3:
                cout << "TRIANGLE" <<endl;
                break;
            case 4:
                cout << "CIRCLE" <<endl;
                break;
            default:
                break;
        }
        
//      cout << "maximum likelihood is " << maximumLikelihood << endl;
    }
    
    cout << classDistances[0] << "," << classDistances[1] << endl;

    
    return EXIT_SUCCESS;
};