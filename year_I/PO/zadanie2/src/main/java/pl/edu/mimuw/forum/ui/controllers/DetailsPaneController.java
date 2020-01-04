package pl.edu.mimuw.forum.ui.controllers;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import pl.edu.mimuw.forum.ui.models.CommentViewModel;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;
import pl.edu.mimuw.forum.ui.models.SurveyViewModel;
import pl.edu.mimuw.forum.ui.models.TaskViewModel;
import pl.edu.mimuw.forum.undoRedoManager.UndoRedoManager;

public class DetailsPaneController implements Initializable {

	@FXML
	private ContentPaneController contentController;
	
	@FXML
	private SuggestionPaneController suggestionController;
	
	@FXML
	private TaskPaneController taskController;
	
	@FXML
	private SurveyPaneController surveyController;
	
	private NodeViewModel model;
	
	public NodeViewModel getModel() {
		return model;
	}

	
	public void setModel(NodeViewModel model) {
		contentController.setModel(null);
		suggestionController.setModel(null);
		taskController.setModel(null);
		surveyController.setModel(null);
		
		if (model != null) {
			this.model = model;
			model.presentOn(this);
		}
	}
	
	public void present(NodeViewModel model) {
		contentController.setModel(model);
	}
	
	public void present(CommentViewModel comment) {
		present((NodeViewModel) comment);
	}
	
	public void present(SuggestionViewModel suggestion) {
		present((NodeViewModel) suggestion);
		suggestionController.setModel(suggestion);
	}
	
	public void present(TaskViewModel task) {
		present((NodeViewModel) task);
		taskController.setModel(task);
	}
	
	public void present(SurveyViewModel survey) {
		present((NodeViewModel) survey);
		surveyController.setModel(survey);
	}

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		setModel(null);
	}
	
	public void passUndoRedoManager(UndoRedoManager undoRedoManager) {
		contentController.setUndoRedoManager(undoRedoManager);
		suggestionController.setUndoRedoManager(undoRedoManager);
		taskController.setUndoRedoManager(undoRedoManager);
		surveyController.setUndoRedoManager(undoRedoManager);
	}

}
