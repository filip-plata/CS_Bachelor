package pl.edu.mimuw.forum.undoRedoManager;

import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.tree.ForumTreeItem;

public class ChangeDataAdd extends ChangeDataTree {
	
	public final boolean wasAdded;

	public ChangeDataAdd(NodeViewModel child, ForumTreeItem node, int position) {
		super(child, node, position);
		this.wasAdded = true;
	}

	@Override
	public void makeChange(MainPaneController pane) throws ApplicationException {
		pane.addNode(viewNode , child);
	}

	@Override
	public void revertChange(MainPaneController pane) throws ApplicationException{
		pane.deleteNode(viewNode.getNode(), child);
	}

}
