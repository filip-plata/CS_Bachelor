package pl.edu.mimuw.forum.ui.controllers;

import java.net.URL;
import java.util.ResourceBundle;

import pl.edu.mimuw.forum.undoRedoManager.UndoRedoManager;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;

public class BasePaneController implements Initializable {

	private SimpleBooleanProperty hasModel;
	protected UndoRedoManager undoRedoManager;

	@FXML
	private Node view;

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		hasModel = new SimpleBooleanProperty(false);

		view.managedProperty().bind(view.visibleProperty());
		view.visibleProperty().bind(hasModel);

		setHasModel(false);
	}

	protected void setHasModel(boolean flag) {
		hasModel.set(flag);
	}

	protected Node getView() {
		return view;
	}
	
	public void setUndoRedoManager(UndoRedoManager undoRedoManager) {
		this.undoRedoManager = undoRedoManager;
	}

}
