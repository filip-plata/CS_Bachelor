package pl.edu.mimuw.forum.ui.controllers;

import java.awt.TextField;
import java.net.URL;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Optional;
import java.util.ResourceBundle;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Dialog;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.stage.Window;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.CommentViewModel;
import pl.edu.mimuw.forum.ui.models.TaskViewModel;
import pl.edu.mimuw.forum.ui.models.SurveyViewModel;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;
import pl.edu.mimuw.forum.ui.controllers.DetailsPaneController;

public class DialogController implements Initializable {

	@FXML
	private Dialog<NodeViewModel> dialog;
	
	//@FXML
	//private ComboBox<String> ComboBox;
	
	@FXML
	private DetailsPaneController detailsController;

	private Window window;

	@Override
	public void initialize(URL x, ResourceBundle arg1) {
		
		window = dialog.getDialogPane().getScene().getWindow();
		window.setOnCloseRequest((event) -> window.hide());

		dialog.setTitle("Post");
		
		ArrayList<String> choices = new ArrayList<>();
		choices.add("comment");
		choices.add("task");
		choices.add("survey");
		choices.add("suggestion");
		
		ChoiceDialog<String> chooseType = new ChoiceDialog<String>("comment", choices);
		
		chooseType.setTitle("Choose content type");
		chooseType.setHeaderText("Make a choice:");
		
		CommentViewModel comment = new CommentViewModel("Some text", "Anonymous");
		SurveyViewModel survey = new SurveyViewModel("Some text", "Anonymous");
		TaskViewModel task = new TaskViewModel("Some text", "Anonymous", new Date());
		SuggestionViewModel suggestion = new SuggestionViewModel("Some text", "Anonymous",
					"No response yet...");
		
		dialog.getDialogPane()
		.getButtonTypes()
		.addAll(ButtonType.CANCEL, ButtonType.OK);
		
		Optional<String> result = chooseType.showAndWait();
		if (result.isPresent()) {
			switch(result.get()) {
				case "comment":
					detailsController.setModel(comment);
					break;
				case "suggestion":
					detailsController.setModel(suggestion);
					break;
				case "task":
					detailsController.setModel(task);
					break;
				case "survey":
					detailsController.setModel(survey);
					break;
			}
		}
		
		dialog.setResultConverter(buttonType -> {
			if (buttonType == ButtonType.OK) {
				return detailsController.getModel();
			}
			return null;
		});
		
		//detailsController.setModel(comment);

		
		/*CommentViewModel comment = new CommentViewModel("Some text", "Anonymous");
		SurveyViewModel survey = new SurveyViewModel("Some text", "Anonymous");
		TaskViewModel task = new TaskViewModel("Some text", "Anonymous", new Date());
		SuggestionViewModel suggestion = new SuggestionViewModel("Some text", "Anonymous",
					"No response yet...");*/
		
		
		//System.out.println(detailsPaneController);
		
		
		
		/*dialog.setResultConverter(buttonType -> {
			if (buttonType == ButtonType.OK) {
				String chosen = ComboBox.getValue();
				
				if (chosen.equals("Comment")) {
					return comment;
				}
				if (chosen.equals("Task")) {
					return task;
				}
				if (chosen.equals("Survey")) {
					return survey;
				}
				if (chosen.equals("Suggestion")) {
					return suggestion;
				}
				return null;
			}
			return null;
		});*/
		
		/*ComboBox.valueProperty().addListener(new ChangeListener<String>() {
			@Override
			public void changed(ObservableValue ov, String t, String t1) {
		            System.out.println(t);
		            System.out.println(t1);
		        }    
		});
		
		CommentViewModel comment = new CommentViewModel("Some text", "Anonymous");
		SurveyViewModel survey = new SurveyViewModel("Some text", "Anonymous");
		TaskViewModel task = new TaskViewModel("Some text", "Anonymous", new Date());
		SuggestionViewModel suggestion = new SuggestionViewModel("Some text", "Anonymous",
					"No response yet...");*/
		
		//dialog.getItems().add(comment);
		//dialog.getItems().add(task);
		//dialog.getItems().add(comment);
		//dialog.getItems().add(comment);
		
		
		
		//dialog
		
		/*CommentViewModel comment = new CommentViewModel("Some text", "Anonymous");
		SurveyViewModel survey = new SurveyViewModel("Some text", "Anonymous");
		TaskViewModel task = new TaskViewModel("Some text", "Anonymous", new Date());
		SuggestionViewModel suggestion = new SuggestionViewModel("Some text", "Anonymous",
					"No response yet...");*/
		
		//final ButtonType button = new ButtonType("Add");
		//final TextField author = new TextField("Author");
		//final TextArea authorName = new TextArea("Anonymus");
		//final ComboBox<NodeViewModel> postsComboBox= new ComboBox<NodeViewModel>();
		
		/*postsComboBox.getItems().addAll(
				comment,
				survey,
				task,
				suggestion
				);
		
		//postsComboBox.setPrefWidth(40.0);
		//postsComboBox.setPrefHeight(30.0);
		
		postsComboBox.setMinWidth(100.0);
		postsComboBox.setMinHeight(60.0);
		postsComboBox.setEditable(true);
		postsComboBox.setPrefWidth(postsComboBox.getMinWidth());
		postsComboBox.setPrefHeight(postsComboBox.getMinHeight());
		postsComboBox.setEditable(false);
		
		dialog.getDialogPane().setContent(postsComboBox);*/
		
		/*dialog.getDialogPane()
		.getButtonTypes()
		.addAll(ButtonType.CANCEL, ButtonType.OK);
		
		
		dialog.setResultConverter(buttonType -> {
			if (buttonType == ButtonType.OK) {
				return null;
			}
			return null;
		});*/
		
		/*commentButtonType = new ButtonType("Comment");
		taskButtonType = new ButtonType("Task");
		surveyButtonType = new ButtonType("Survey");
		suggestionButtonType = new ButtonType("Suggestion");

		dialog.getDialogPane()
				.getButtonTypes()
				.addAll(ButtonType.CANCEL, commentButtonType, taskButtonType,
						suggestionButtonType, surveyButtonType);

		dialog.setResultConverter(buttonType -> {
			NodeViewModel result = null;

			if (buttonType == commentButtonType) {
				result = new CommentViewModel("Some text", "Anonymous");
			}
			if (buttonType == surveyButtonType) {
				result = new SurveyViewModel("Some text", "Anonymous");
			}
			if (buttonType == taskButtonType) {
				result = new TaskViewModel("Some text", "Anonymous", new Date());
			}
			if (buttonType == suggestionButtonType) {
				result = new SuggestionViewModel("Some text", "Anonymous",
						"No response yet...");
			}

			return result;
		});*/

	}

}
