package pl.edu.mimuw.forum.data;

import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;

/**
 *
 * @author jsroka
 */
public class Suggestion extends Node {
	String response;
	Boolean isResponseAccepted;

	public Suggestion(String content, String author, String response) {
		super(content, author);
		this.response = response;
	}

	public String getResponse() {
		return response;
	}

	public void setResponse(String response) {
		this.response = response;
		this.isResponseAccepted = null;
	}

	public Boolean getIsResponseAccepted() {
		return isResponseAccepted;
	}

	public void setIsResponseAccepted(Boolean isResponseAccepted) {
		this.isResponseAccepted = isResponseAccepted;
	}

	@Override
	protected NodeViewModel createModel() {
		return new SuggestionViewModel(this);
	}

	@Override
	public String toString() {
		return "Suggestion(" + super.toString() + "response=" + response + ", responseAccepted=" + isResponseAccepted
				+ ')';
	}

}
