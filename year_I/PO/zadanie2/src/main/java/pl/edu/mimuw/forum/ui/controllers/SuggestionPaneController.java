package pl.edu.mimuw.forum.ui.controllers;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.beans.property.StringProperty;
import javafx.fxml.FXML;
import javafx.scene.control.TextArea;
import jfxtras.styles.jmetro8.ToggleSwitch;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;
import pl.edu.mimuw.forum.undoRedoManager.ChangeDataTextField;

public class SuggestionPaneController extends BasePaneController {

	private SuggestionViewModel model;

	@FXML
	private TextArea suggestionField;

	@FXML
	private ToggleSwitch acceptableField;

	public void setModel(SuggestionViewModel model) {
		if (this.model != null) {
			suggestionField.textProperty().unbindBidirectional(this.model.getResponse());
			acceptableField.selectedProperty().unbindBidirectional(this.model.getIsResponseAccepted());
		}

		this.model = model;

		if (this.model != null) {
			suggestionField.textProperty().bindBidirectional(this.model.getResponse());
			acceptableField.selectedProperty().bindBidirectional(this.model.getIsResponseAccepted());
		}

		setHasModel(model != null);
	}
	
	@Override
	public void initialize(URL location, ResourceBundle resources) {
		super.initialize(location, resources);
		
		suggestionProperty().addListener((observable, oldValue, newValue) -> {
			if (undoRedoManager != null) {
				undoRedoManager.memorize(new ChangeDataTextField(model, oldValue, newValue, this.suggestionProperty()));
			}
		});
		
	}
	
	public StringProperty suggestionProperty() {
		return suggestionField.textProperty();
	}

}
