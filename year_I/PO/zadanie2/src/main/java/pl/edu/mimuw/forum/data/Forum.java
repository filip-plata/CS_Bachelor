package pl.edu.mimuw.forum.data;

import pl.edu.mimuw.forum.ui.models.NodeViewModel;

public class Forum extends Node {
	public Forum(String content, String author) {
		super(content, author);
	}

	@Override
	public NodeViewModel getModel() {
		return this.getChildren().get(0).getModel();
	}
	
	public Forum(Node node) {
		super(null, null);
		this.children.add(node);
	}
}
