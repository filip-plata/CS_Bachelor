package Hotel.Client;

public class PersonWithStrategy {
	protected String name;
	protected String surname;
	//private Strategy strategy;
	protected PersonWithStrategy(String name, String surname){
		this.name = name;
		this.surname = surname;
	}
	
	@Override
	public String toString(){
		return name + " " + surname;
	}
}
