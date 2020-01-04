package pl.edu.mimuw.forum.data;

import java.util.LinkedList;
import java.util.List;

//import com.thoughtworks.xstream.annotations.XStreamOmitField;

import pl.edu.mimuw.forum.ui.models.NodeViewModel;

/**
 *
 * @author jsroka
 */
public class Node {
	String content;
	String author;
	List<Node> children = new LinkedList<>();

	//@XStreamOmitField
	transient NodeViewModel model;

	public Node(String content, String author) {
		this.content = content;
		this.author = author;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public String getAuthor() {
		return author;
	}

	public void setAuthor(String autor) {
		this.author = autor;
	}

	public List<? extends Node> getChildren() {
		return children;
	}

	public void addChild(Node child) {
		children.add(child);
	}

	public void removeChild(Node child) {
		children.remove(child);
	}

	public NodeViewModel getModel() {
		if (model == null) {
			model = createModel();
		}
		return model;
	}

	protected NodeViewModel createModel() {
		return new NodeViewModel(this);
	}

	@Override
	public String toString() {
		return "Content: " + content + "author: " + author;
	}

}
