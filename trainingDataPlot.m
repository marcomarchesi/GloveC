
% TRAINING DATA ANALYZER 

f1 = fileread('TrainingData.txt');
splitData = regexp(f1,'************TIME_SERIES************','split');
splitData = splitData(:,2:end);
samplesSize = size(splitData,2);
sampleArray = zeros(samplesSize,6);

for i=1:samplesSize
    % return the class of each sample
    [classes,class_tokens] = regexp(f1,'ClassID:\s([0-9])','tokens','match');
    [sample,sample_tokens] = regexp(splitData(1,i),'TimeSeriesData:(.*)','tokens','match');
    % 6-dimensional sample
    if(i==10)
        data = str2num(char(sample{1,1}{1,1}));
%         plot(data(:,1:3));  % only accelerometer data
%         plot(data(:,4:6));  % only gyroscope data
        plot(data);
    end
end

