package Hotel;


public class RoomAtributes {
	
	public enum InteriorDesign {
		ORIENTALNY,
		MORSKI,
		NOWOCZESNY,
		RUSTYKALNY,
		SECESYJNY
	}
	
	public enum Color {
		SZARY,
		STALOWY,
		PURPUROWY,
		MORSKI,
		SELEDYNOWY,
		JASNOZIELONY
	}
	
	public enum WindowAlingment {
		Północ,
		Południe,
		Wschód,
		Zachód
	}
	
	public final static InteriorDesign[] styles = {InteriorDesign.RUSTYKALNY, InteriorDesign.MORSKI,
		InteriorDesign.NOWOCZESNY, InteriorDesign.ORIENTALNY, InteriorDesign.SECESYJNY};
	
	public final static Color[] colors = {Color.SELEDYNOWY, Color.SZARY, Color.JASNOZIELONY, 
		Color.MORSKI, Color.PURPUROWY, Color.STALOWY};
	
	public final static WindowAlingment[] directions = {WindowAlingment.Wschód, WindowAlingment.Północ,
		WindowAlingment.Południe, WindowAlingment.Zachód};
	
}
