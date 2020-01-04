package pl.edu.mimuw.forum.exceptions;

public class ApplicationException extends Exception {

	private static final long serialVersionUID = 1L;
	
	public ApplicationException() {
		super();
	}
	
	public ApplicationException(String message, Throwable cause) {
		super(message, cause);
	}
	
	public ApplicationException(Throwable cause) {
		super(cause);
	}

}
