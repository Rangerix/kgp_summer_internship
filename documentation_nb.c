naive bayes:
	for each column in the dataset: get all the unique values
	for each feature:
		for each unique value:
			get the count for label1 and label2
			divide the count by #label1 and #label2
		this will give the probability of all the unique values given a label value 

	now, 
	for test matrix:
		for each testsample:
			we have to calculate P(label1/(the feature values)) and P(label2/(the feature values))
			P(label1/(the feature values))=P(value_feature1/label1)*P(value_feature2/label1)*...*P(value_featuren/label1)*P(label1)
			P(label2/(the feature values))=P(value_feature1/label2)*P(value_feature2/label2)*...*P(value_featuren/label2)*P(label2)
			
			all such probabilities we have already calculated for training matrix; we have to get the values from there
			
		if(P(label1/the feature values)>P(label2/the feature values))
			the predicted label for this testsample: label1
		else
			the predicted label for this testsample: label2