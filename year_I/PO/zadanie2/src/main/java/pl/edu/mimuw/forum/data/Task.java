package pl.edu.mimuw.forum.data;

import java.util.Date;

import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.TaskViewModel;

/**
 *
 * @author jsroka
 */
public class Task extends Node {
	Date dueDate;

	public Task(Date dueDate, String content, String author) {
		super(content, author);
		this.dueDate = dueDate;
	}

	public Date getDueDate() {
		return dueDate;
	}

	public void setDueDate(Date dueDate) {
		this.dueDate = dueDate;
	}

	protected NodeViewModel createModel() {
		return new TaskViewModel(this);
	}

	@Override
	public String toString() {
		return "Task(" + super.toString() + " due date: " + dueDate + ")";
	}
}
