package pl.edu.mimuw.forum.data;

import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.SurveyViewModel;

/**
 *
 * @author jsroka
 */
public class Survey extends Node {

	int likes, dislikes;

	public Survey(String content, String author) {
		this(content, author, 0, 0);
	}

	public Survey(String content, String author, int likes, int dislikes) {
		super(content, author);
		this.likes = likes;
		this.dislikes = dislikes;
	}

	public int getLikes() {
		return likes;
	}

	public int getDislikes() {
		return dislikes;
	}

	public void addLike() {
		likes++;
	}

	public void removeLike() {
		likes--;
		if (likes < 0) {
			likes = 0;
		}
	}

	public void addDislike() {
		dislikes--;
	}

	public void removeDislike() {
		dislikes++;
		if (dislikes < 0) {
			dislikes = 0;
		}
	}

	@Override
	protected NodeViewModel createModel() {
		return new SurveyViewModel(this);
	}

	@Override
	public String toString() {
		return "Surve(" + super.toString() + " likes=" + likes
				+ ", dislikes=" + dislikes + ')';
	}
}
