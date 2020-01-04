package Hotel;


import java.awt.print.Printable;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Random;

import org.junit.Test;

import Hotel.Client.Client;
import Hotel.Client.ClientBudget;
import Hotel.Client.ClientDovish;
import Hotel.Client.ClientHalf;
import Hotel.Client.ClientPerfectionist;
import Hotel.Client.ClientViewer;
import Hotel.DeskClerk.AproximatingDeskClerk;
import Hotel.DeskClerk.DeskClerk;
import Hotel.DeskClerk.PerfectDeskClerk;
import Hotel.DeskClerk.SnarkyDeskClerk;



/*import edu.mimuw.students.an371233.hotel.Hotel;
import edu.mimuw.students.an371233.hotel.Order;
import edu.mimuw.students.an371233.hotel.Period;
import edu.mimuw.students.an371233.hotel.Room;
import edu.mimuw.students.an371233.hotel.RoomDescription;
import edu.mimuw.students.an371233.hotel.RoomDescription.Color;
import edu.mimuw.students.an371233.hotel.RoomDescription.Direction;
import edu.mimuw.students.an371233.hotel.RoomDescription.RoomDescriptionBuilder;
import edu.mimuw.students.an371233.hotel.RoomDescription.Style;
import edu.mimuw.students.an371233.hotel.Survey;
import edu.mimuw.students.an371233.hotel.clients.AcquiescentClient;
import edu.mimuw.students.an371233.hotel.clients.BudgetClient;
import edu.mimuw.students.an371233.hotel.clients.Client;
import edu.mimuw.students.an371233.hotel.clients.HalfMatcherClient;
import edu.mimuw.students.an371233.hotel.clients.PerfectionistClient;
import edu.mimuw.students.an371233.hotel.clients.ViewerClient;
import edu.mimuw.students.an371233.hotel.receptionists.ApproximateReceptionist;
import edu.mimuw.students.an371233.hotel.receptionists.PerfectionistReceptionist;
import edu.mimuw.students.an371233.hotel.receptionists.RandomReceptionist;
import edu.mimuw.students.an371233.hotel.receptionists.Receptionist;
import edu.mimuw.students.an371233.hotel.receptionists.UglyReceptionist;
import edu.mimuw.students.an371233.hotel.utility.Printable;*/

// TODO: Auto-generated Javadoc
/**
 * The Class HotelTest.
 */
public class HotelTest {

	Random random = new Random();

	public static int TEST_SIZE = 10;

	// test is determined by pair of those values
	public static int TEST_NUMBER = 0;

	/**
	 * Should return false third time.
	 */
	/*@Test
	public void shouldReturnFalseThirdTime() {
		Order order = new Order(new ClientBudget(null, null), new Questionnaire(null,
				null));

		// Assert.assertTrue(order.increaseCount());
		// Assert.assertTrue(order.increaseCount());
		// Assert.assertFalse(order.increaseCount());
	}

	@Test
	public void shouldThrowException() {
		RoomDescriptionBuilder builder = new RoomDescriptionBuilder();
		try {
			builder.build();
		} catch (RuntimeException e) {
			// e.printStackTrace();
			return;
		}
		Assert.assertTrue(false);
	}

	@Test
	public void shouldNotThrowException() {

		RoomDescriptionBuilder builder = new RoomDescriptionBuilder();

		builder.hasInternetAccess(false).setColor(Color.CELADON)
				.setDirection(Direction.EAST).setNumberOfResidents(4)
				.setPrice(1000).setStyle(Style.MODERN).build();
	}*/

	@Test
	public void test() {

		random.setSeed(TEST_NUMBER);

		System.out.println(String.format("TEST: %d, SIZE: %d\n", TEST_NUMBER,
				TEST_SIZE));

		Hotel hotel = new Hotel(null, null);

		Room[] rooms = getSomeRooms(random.nextInt(10 * TEST_SIZE) + TEST_SIZE);
		DeskClerk[] rec = getSomeReceptionists(random.nextInt(TEST_SIZE) + 1);

		int ordersCount = random.nextInt(10 * TEST_SIZE) + TEST_SIZE;
		Client[] clients = getSomeClients(ordersCount);
		Order[] orders = makeOrders(ordersCount, clients,
				getSomeSurveys(ordersCount));

		hotel.accept(orders, rooms, rec);

		// orders[0] = new Order();
	}

	private Order[] makeOrders(int i, Client[] clients, Questionnaire[] someSurveys) {
		Order[] orders = new Order[i];

		for (int id = 0; id < i; id++) {
			orders[id] = new Order(clients[id], someSurveys[id]);
		}

		return orders;
	}
	
	private class Para{
		public String data;
		public int stayPeriod;
		
		public Para(String x, int a){
			this.data = x;
			this.stayPeriod = a;
		}
	}

	private Questionnaire[] getSomeSurveys(int i) {
		Questionnaire[] surveys = new Questionnaire[i];
		Room[] desc = getSomeRoomDescriptions(i);
		Para[] periods = getSomePeriods(i);
		Questionnaire.QuestionnaireBuilder heh = new Questionnaire.QuestionnaireBuilder();

		for (int j = 0; j < i; j++) {
			heh.setInteriorDesign(desc[j].getInteriorDesign());
			heh.setColor(desc[j].getColor());
			heh.setWindowAlingment(desc[j].getWindowAlingment());
			heh.setInternetAccess(desc[j].getInternetAccess());
			heh.setStayPeriod(periods[j].data, periods[j].stayPeriod);
			heh.setPeopleCapacity(desc[j].getPrice());
			surveys[j] = heh.build();
		}
		
		return surveys;
	}

	private Para[] getSomePeriods(int i) {

		Calendar cal = Calendar.getInstance();

		Para[] periods = new Para[i];

		SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");

		for (int j = 0; j < i; j++) {
			cal.setTimeInMillis(1000000L + random.nextInt(1000 * 60 * 60 * 24
					* 360));
			periods[j] = new Para(format.format(cal.getTime()), random.nextInt(TEST_SIZE*2));
		}

		return periods;
	}

	private Room[] getSomeRoomDescriptions(int i) {
		Room[] desc = new Room[i];

		Room.RoomBuilder builder = new Room.RoomBuilder();

		for (int j = 0; j < i; j++) {
			desc[j] = builder.setInternetAccess(random.nextBoolean())
					.setColor(getRandColor()).setWindowAlingment(getRandDirection())
					.setPrice(random.nextInt(TEST_SIZE) + 1)
					.setInteriorDesign(getRandStyle()).build();
		}

		return desc;
	}


	public RoomAtributes.Color getRandColor() {
		int i = RoomAtributes.colors.length;
		RoomAtributes.Color res = RoomAtributes.colors[random.nextInt(i)];
		return res;
	}

	public RoomAtributes.WindowAlingment getRandDirection() {
		int i = RoomAtributes.directions.length;
		RoomAtributes.WindowAlingment res = RoomAtributes.directions[random.nextInt(i)];
		return res;
	}

	public RoomAtributes.InteriorDesign getRandStyle() {
		int i = RoomAtributes.styles.length;
		RoomAtributes.InteriorDesign res = RoomAtributes.styles[random.nextInt(i)];
		
		return res;
	}
	

	public Printable getRand(Printable[] p) {
		return p[random.nextInt(p.length)];
	}

	private Room[] getSomeRooms(int i) {
		Room[] rooms = new Room[i];
		//System.out.println(random.nextInt(1000));
		Room[] desc = getSomeRoomDescriptions(i);

		for (int j = 0; j < i; j++) {
			rooms[j] = new Room(desc[j], j);
		}

		return rooms;
	}

	String names[] = { "Mariusz", "Zdzis�aw", "Bo�ydar", "Janusz", "Tadeusz",
			"Stanis�aw", "W�odzimierz", "Bogdan" };

	private DeskClerk[] getSomeReceptionists(int i) {
		DeskClerk[] rec = new DeskClerk[i];

		for (int j = 0; j < i; j++) {
			rec[j] = getRandomReceptionist();
		}

		return rec;
	}

	private DeskClerk getRandomReceptionist() {
		int i = random.nextInt(3);

		switch (i) {
		case 0:
			return new AproximatingDeskClerk("Najkorzystniejszy",
					getRandomName());
		case 1:
			return new PerfectDeskClerk("Perfekcyjny", getRandomName());
		//case 2:
			//return new RandomReceptionist("Jakminajdzie", getRandomName());
		case 2:
			return new SnarkyDeskClerk("Podw�rkowy", getRandomName());

		default:
			return null; // never happens
		}
	}

	private String getRandomName() {
		return names[random.nextInt(names.length)];
	}

	private Client[] getSomeClients(int i) {
		Client[] client = new Client[i];

		for (int j = 0; j < i; j++) {
			client[j] = getRandomClient();
		}

		return client;
	}

	private Client getRandomClient() {
		int i = random.nextInt(5);

		switch (i) {
		case 0:
			return new ClientBudget("Oszcz�dny", getRandomName());
		case 1:
			return new ClientHalf("Po�owiczny", getRandomName());
		case 2:
			return new ClientViewer("Widokowy", getRandomName());
		case 3:
			return new ClientDovish("Wszystkomijedno", getRandomName());
		case 4:
			return new ClientPerfectionist("Wdech�", getRandomName());
		}
		return null; // never happens
	}
}
