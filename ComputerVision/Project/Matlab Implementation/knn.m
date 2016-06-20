results = nn_classification_PhD(model.train, train_id, test_features, test_id, size(test_features,1), 'mahcos');

output = evaluate_results_PhD(results,'ID');
figure(1)
plot_ROC_PhD(output.ROC_ver_rate, output.ROC_miss_rate,'r',2);
title('ROC curve for the GAB+LDA+MAHCOS technique on the ORL database.')
legend('GAB+LDA+MAHCOS')
figure(2)
plot_CMC_PhD(output.CMC_rec_rates , output.CMC_ranks,'r',2);
legend('GAB+LDA+MAHCOS')
title('CMC curve for the GAB+LDA+MAHCOS technique on the ORL database.')
