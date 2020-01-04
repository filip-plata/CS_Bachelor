package pl.edu.mimuw.forum.undoRedoManager;

import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;

public interface UndoRedoOperation {

	public abstract void makeChange(MainPaneController pane) throws ApplicationException;
	public abstract void revertChange(MainPaneController pane) throws ApplicationException;
	public abstract boolean merge(UndoRedoOperation undoRedoOperation);
}
