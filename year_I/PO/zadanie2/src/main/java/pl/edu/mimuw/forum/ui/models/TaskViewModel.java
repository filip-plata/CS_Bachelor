package pl.edu.mimuw.forum.ui.models;

import java.util.Date;

import javafx.beans.property.SimpleObjectProperty;
import pl.edu.mimuw.forum.data.Node;
import pl.edu.mimuw.forum.data.Task;
import pl.edu.mimuw.forum.ui.controllers.DetailsPaneController;

public class TaskViewModel extends NodeViewModel {

	public static final String NAME = "Task";

	private final SimpleObjectProperty<Date> dateProperty;

	public TaskViewModel(String content, String author, Date date) {
		this(new Task(date, content, author));
	}
	
	public TaskViewModel(Task task) {
		super(task);

		dateProperty = new SimpleObjectProperty<>(task.getDueDate());
	}
	
	public SimpleObjectProperty<Date> getDueDate() {
		return dateProperty;
	}

	@Override
	protected Node createDocument() {
		return new Task(dateProperty.get(), getContent().get(), getAuthor().get());
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
