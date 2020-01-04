package pl.edu.mimuw.forum.ui.models;

import java.util.Optional;

import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import pl.edu.mimuw.forum.data.Suggestion;
import pl.edu.mimuw.forum.data.Node;
import pl.edu.mimuw.forum.ui.controllers.DetailsPaneController;

public class SuggestionViewModel extends NodeViewModel {

	public static final String NAME = "Suggestion";
	
	private final SimpleStringProperty responseProperty;
	private final SimpleBooleanProperty isResponseAccepted;
	//private final SimpleBooleanPrzoperty isIndeterminateProperty;

	public SuggestionViewModel(String content, String author, String response) {
		this(new Suggestion(content, author, response));
	}

	public SuggestionViewModel(Suggestion suggestion) {
		super(suggestion);

		responseProperty = new SimpleStringProperty(suggestion.getResponse());

		isResponseAccepted = new SimpleBooleanProperty(
				Optional.ofNullable(suggestion.getIsResponseAccepted()).orElse(false));
		//isIndeterminateProperty = new SimpleBooleanProperty(sugestia.isAutorUsatysfakcjonowany() == null);
	}

	public StringProperty getResponse() {
		return responseProperty;
	}
	
	public BooleanProperty getIsResponseAccepted() {
		return isResponseAccepted;
	}
	
	@Override
	protected Node createDocument() {
		Suggestion s = new Suggestion(getContent().get(), getAuthor().get(), responseProperty.get());
		s.setIsResponseAccepted(isResponseAccepted.get());
		return s;
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
