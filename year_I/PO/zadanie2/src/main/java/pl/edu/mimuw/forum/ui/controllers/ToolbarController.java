package pl.edu.mimuw.forum.ui.controllers;

import javafx.beans.binding.Bindings;
import javafx.beans.value.ObservableBooleanValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import pl.edu.mimuw.forum.ui.bindings.ToolbarBindings;

public class ToolbarController {

	@FXML
	private Button newButton;

	@FXML
	private Button openButton;

	@FXML
	private Button saveButton;

	@FXML
	private Button undoButton;

	@FXML
	private Button redoButton;

	@FXML
	private Button addNodeButton;

	@FXML
	private Button deleteNodeButton;

	void bind(ToolbarBindings bindings) {
		Button buttons[] = { newButton, openButton, saveButton, undoButton, redoButton, addNodeButton,
				deleteNodeButton };
		Runnable actions[] = { bindings.getOnNew(), bindings.getOnOpen(), bindings.getOnSave(), bindings.getOnUndo(),
				bindings.getOnRedo(), bindings.getOnAddNode(), bindings.getOnDeleteNode() };
		ObservableBooleanValue enabled[] = { bindings.getCanCreateNew(), bindings.getCanOpen(), bindings.getCanSave(),
				bindings.getCanUndo(), bindings.getCanRedo(), bindings.getCanAddNode(), bindings.getCanDeleteNode() };

		for (int i = 0; i < buttons.length; ++i) {
			buttons[i].setOnAction(FromRunnable(actions[i]));
			buttons[i].disableProperty().bind(Bindings.not(enabled[i]));
		}

	}

	private static EventHandler<ActionEvent> FromRunnable(Runnable procedure) {
		return evt -> procedure.run();
	}

}
