package pl.edu.mimuw.forum.data;

import pl.edu.mimuw.forum.ui.models.CommentViewModel;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;

/**
 *
 * @author jsroka
 */
public class Comment extends Node {

	public Comment(String content, String author) {
		super(content, author);
	}

	@Override
	protected NodeViewModel createModel() {
		return new CommentViewModel(this);
	}

	@Override
	public String toString() {
		return "Comment(" + super.toString() + ")";
	}
}
