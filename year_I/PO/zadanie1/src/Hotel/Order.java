package Hotel;
import Hotel.Client.Client;


public class Order {
	private Client client;
	private Questionnaire questionnaire;
	private int reheard;
	
	public int getReheard(){
		return this.reheard;
	}
	
	public Order(Client client, Questionnaire questionnaire) {
		this.client = client;
		this.questionnaire = questionnaire;
		this.reheard = 0;
	}
	
	public void reheardAgain(){
		reheard++;
	}
	
	public boolean reheardTooManyTimes(){
		return (reheard >= 3) ? true : false;
	}
	
	public Questionnaire getQuestionnaire(){
		return questionnaire;
	}
	
	public Client getClient(){
		return client;
	}
	
	@Override
	public String toString(){
		return this.questionnaire.toString();
	}
}
