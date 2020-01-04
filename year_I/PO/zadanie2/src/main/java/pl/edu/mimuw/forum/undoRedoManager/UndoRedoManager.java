package pl.edu.mimuw.forum.undoRedoManager;

import java.util.ArrayList;

import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;

public class UndoRedoManager {
	
	private ArrayList<UndoRedoOperation> changesStack;
	private int changesIterator;
	private MainPaneController mainPaneController;
	
	private boolean dummyChanges;
	
	public UndoRedoManager(MainPaneController myController) {
		dummyChanges = false;
		changesStack = new ArrayList<UndoRedoOperation>();
		changesIterator = 0;
		mainPaneController = myController;
	}
	
	public void memorize(UndoRedoOperation change) {
		if (! dummyChanges) {
			for (int j=changesIterator; j<changesStack.size(); j++) {
				changesStack.remove(j);
			}
			
			if (changesIterator > 0 && change.merge(changesStack.get(changesIterator - 1))) {
				changesStack.remove(changesIterator - 1);
				changesIterator -= 1;
			}
			changesIterator += 1;
			changesStack.add(change);
			
			mainPaneController.getBindings().undoAvailableProperty().set(true);
			mainPaneController.getBindings().redoAvailableProperty().set(false);
		}
	}
	
	public void undoLastChange() throws ApplicationException {
		dummyChanges = true;
		
		UndoRedoOperation change = changesStack.get(changesIterator - 1);
		change.revertChange(mainPaneController);
		changesIterator -= 1;
		
		if ( (changesIterator) == 0) {
			mainPaneController.getBindings().undoAvailableProperty().set(false);
		}
		
		mainPaneController.getBindings().redoAvailableProperty().set(true);
		
		dummyChanges = false;
	}
	
	public void redoLastChange() throws ApplicationException {
		dummyChanges = true;
		
		UndoRedoOperation change = changesStack.get(changesIterator);
		change.makeChange(mainPaneController);
		changesIterator++;
		
		mainPaneController.getBindings().undoAvailableProperty().set(true);
		
		if (changesIterator == changesStack.size()) {
			mainPaneController.getBindings().redoAvailableProperty().set(false);
		}
		
		dummyChanges = false;
	}
	
	public void setDummyAction() {
		dummyChanges = true;
	}
	
	public void unsetDummyAction() {
		dummyChanges = false;
	}

	public void clearHistory() {
		changesStack.clear();
		changesIterator = 0;
		
		mainPaneController.getBindings().undoAvailableProperty().set(false);
		mainPaneController.getBindings().redoAvailableProperty().set(false);
	}
}
