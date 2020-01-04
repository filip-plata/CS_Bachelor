package pl.edu.mimuw.forum.ui.models;

import pl.edu.mimuw.forum.data.Comment;
import pl.edu.mimuw.forum.data.Node;
import pl.edu.mimuw.forum.ui.controllers.DetailsPaneController;

public class CommentViewModel extends NodeViewModel {

	public static final String NAME = "Comment";
	
	public CommentViewModel(String content, String author) {
		this(new Comment(content, author));
	}
	
	public CommentViewModel(Comment comment) {
		super(comment);
	}

	@Override
	protected Node createDocument() {
		return new Comment(getContent().get(), getAuthor().get());
	}
	
	@Override
	public String getName() {
		return NAME;
	}
	
	@Override
	public void presentOn(DetailsPaneController detailsPaneController) {
		detailsPaneController.present(this);
	}
	
}
