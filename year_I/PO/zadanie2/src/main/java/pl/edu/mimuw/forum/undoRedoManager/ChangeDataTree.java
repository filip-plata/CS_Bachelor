package pl.edu.mimuw.forum.undoRedoManager;


import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.tree.ForumTreeItem;

public abstract class ChangeDataTree implements UndoRedoOperation {

	protected final NodeViewModel child;
	protected final ForumTreeItem viewNode;
	protected final int position;
	
	public ChangeDataTree() {
		child = null;
		viewNode = null;
		position = 0;
	}
	
	
	public ChangeDataTree(NodeViewModel child, ForumTreeItem node, int position) {
		this.viewNode = node;
		this.position = position;
		this.child = child;
	}
	
	@Override
	public boolean merge(UndoRedoOperation undoRedoOperation) {
		return false;
	}
	
	public abstract void makeChange(MainPaneController pane) throws ApplicationException;
	public abstract void revertChange(MainPaneController pane) throws ApplicationException;;
	
}
