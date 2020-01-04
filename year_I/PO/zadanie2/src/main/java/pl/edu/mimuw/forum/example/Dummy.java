package pl.edu.mimuw.forum.example;

import java.util.Date;

import pl.edu.mimuw.forum.data.Comment;
import pl.edu.mimuw.forum.data.Node;
import pl.edu.mimuw.forum.data.Suggestion;
import pl.edu.mimuw.forum.data.Survey;
import pl.edu.mimuw.forum.data.Task;

public class Dummy {
	public static Node Create() {
        Node forum = new Comment("Na tym forum będziemy dyskutować o zadaniach.", "Trelemorele");
        Node t1 = new Task(new Date(), "To jest treść zadania.", "Jacek Placek");
        Comment c1 = new Comment("Pierwszy komentarz.", "Student1");
        Comment c2 = new Comment("Drugi komentarz.", "Student2");
        Comment c21 = new Comment("Komentarz do drugiego komentarza.", "Jacek Placek");
        Comment c22 = new Comment("Drugi komentarz do drugiego komentarza.", "Student2");
        Survey s23 = new Survey("Treść ankiety", "Student3");

        Node t2 = new Task(new Date(), "A to jest inne zadanie.", "Autor nieznany");
        Suggestion sug1 = new Suggestion("Treść...", "Student3", "O co to to nie!");
        
        forum.addChild(t1);
        t1.addChild(c1);
        t1.addChild(c2);
        c2.addChild(c21);
        c2.addChild(c22);
        c2.addChild(s23);

        forum.addChild(t2);
        t2.addChild(sug1);
        
        return forum;
	}
}
