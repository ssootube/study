import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Types;

public class Assignment {
	 public static void main(String[] args) {
		    String url="jdbc:mysql://localhost:3306/movies?serverTimezone=UTC&useSSL=false";
		    String id="root";
		    String password="0000";
		    Connection conn=null;
		    Statement stmt=null;
		    ResultSet rs=null;
		    
		    try {
		       Class.forName("com.mysql.cj.jdbc.Driver");
		       conn=DriverManager.getConnection(url, id, password);
		       stmt=conn.createStatement();
		       
		       //테이블 생성
		       stmt.executeUpdate("create table director(" + 
		          		"directorID int not null auto_increment, " + 
		          		"directorName varchar(20), " + 
		          		"dateOfBirth varchar(10), " + 
		          		"dateOfDeath varchar(10)," + 
		          		"primary key (directorID))");
		       stmt.executeUpdate("create table actor("
		       		+ "actorID int not null auto_increment, "
		       		+ "actorName varchar(20), "
		       		+ "dateOfBirth varchar(10), "
		       		+ "dateOfDeath varchar(10), "
		       		+ "gender char(1),"
		       		+ "primary key (actorID))");
		       stmt.executeUpdate("create table movie(" + 
		       		"movieID int not null auto_increment," + 
		       		"movieName varchar(20)," + 
		       		"releaseYear int," + 
		       		"releaseMonth int," + 
		       		"releaseDate int," + 
		       		"publisherName varchar(30),"
		       		+ "avgRate numeric(4,2),"+
		       		"primary key (movieID)" + 
		       		")");
		       stmt.executeUpdate("create table award(" + 
		       		"awardID int not null auto_increment, " + 
		       		"awardName varchar(40)," + 
		       		"primary key (awardID)" + 
		       		")");
		       stmt.executeUpdate("create table genre(" + 
		       		"genreName varchar(20)," + 
		       		"primary key (genreName)" + 
		       		")");
		       stmt.executeUpdate("create table movieGenre("
		       		+ "movieID int not null ,"
		       		+ "genreName varchar(20) references genre,"
		       		+ "primary key(movieID,genreName),"
		       		+ "foreign key(movieID) references movie(movieID) on delete cascade "
		       		+ ")");
		       stmt.executeUpdate("create table movieObtain(" + 
		       		"movieID int, " + 
		       		"awardID int references award, " + 
		       		"year int," + 
		       		"primary key (movieID,awardID),"
		       		+ "foreign key(movieID) references movie(movieID) on delete cascade "
		       		+ ")");
		       stmt.executeUpdate("create table actorObtain(" + 
		       		"actorID int references actor, " + 
		       		"awardID int references award, " + 
		       		"year int," + 
		       		"primary key(actorID,awardID))");
		       stmt.executeUpdate("create table directorObtain(" + 
		       		"directorID int references director, " + 
		       		"awardID int references award, " + 
		       		"year int," + 
		       		"primary key(directorID,awardID,year))");
		       stmt.executeUpdate("create table casting(" + 
		       		"movieID int, " + 
		       		"actorID int references actor, " + 
		       		"role varchar(20)," + 
		       		"primary key(movieID,actorID),"
		       		+ "foreign key(movieID) references movie(movieID) on delete cascade "
		       		+ ")");
		       stmt.executeUpdate("create table make(" + 
		       		"movieID int, " + 
		       		"directorID int references director," + 
		       		"primary key(movieID,directorID),"
		       		+ "foreign key(movieID) references movie(movieID) on delete cascade )");
		       stmt.executeUpdate("create table customer(" + 
		       		"customerID int not null auto_increment, " + 
		       		"customerName varchar(20), " + 
		       		"dateOfBirth varchar(10), " + 
		       		"gender char(1)," + 
		       		"primary key(customerID))");
		       stmt.executeUpdate("create table customerRate(" + 
		       		"customerID int references customer, " + 
		       		"movieID int, " + 
		       		"rate numeric(3,1)," + 
		       		"primary key(customerID,movieID),"
		       		+ "foreign key(movieID) references movie(movieID) on delete cascade ,"
		       		+ "foreign key(customerID) references customer(customerID) on delete cascade "
		       		+ ")");
		       
					// 초기 데이터 삽입
					// director
					int count = 1;
					PreparedStatement stmt2 = conn
							.prepareStatement("insert into director(directorName, dateOfBirth, dateOfDeath) values(?,?,?)");
					stmt2.setString(count++, "Tim Burton");
					stmt2.setString(count++, "1958.8.25");
					stmt2.setNull(count, Types.VARCHAR);
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "David Fincher");
					stmt2.setString(count, "1962.8.28");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Christopher Nolan");
					stmt2.setString(count, "1970.7.30");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();

					// actor
					stmt2.close();
					count = 1;
					stmt2 = conn
							.prepareStatement("insert into actor(actorName,dateOfBirth,dateOfDeath,gender) values(?,?,?,?)");
					stmt2.setString(count++, "Johnny Depp");
					stmt2.setString(count++, "1963.6.9");
					stmt2.setNull(count++, Types.VARCHAR);
					stmt2.setString(count, "M");
					stmt2.addBatch();

					stmt2.setString(1, "Winona Ryder");
					stmt2.setString(2, "1971.10.29");
					stmt2.setString(4, "F");
					stmt2.addBatch();

					stmt2.setString(1, "Anne Hathaway");
					stmt2.setString(2, "1982.11.12");
					stmt2.setString(4, "F");
					stmt2.addBatch();

					stmt2.setString(1, "Christian Bale");
					stmt2.setString(2, "1974.1.30");
					stmt2.setString(4, "M");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Heath Ledger");
					stmt2.setString(count++, "1979.4.4");
					stmt2.setString(count++, "2008.1.22");
					stmt2.setString(count, "M");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Jesse Eisenberg");
					stmt2.setString(count++, "1983.10.5");
					stmt2.setNull(count++, Types.VARCHAR);
					stmt2.setString(count++, "M");
					stmt2.addBatch();

					stmt2.setString(1, "Andrew Garfield");
					stmt2.setString(2, "1983.8.20");
					stmt2.setString(4, "M");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();
					// customer
					stmt2.close();
					count = 1;
					stmt2 = conn.prepareStatement("insert into customer(customerName,dateOfBirth,gender) values(?,?,?)");
					stmt2.setString(count++, "Bob");
					stmt2.setString(count++, "1997.11.14");
					stmt2.setString(count++, "M");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "John");
					stmt2.setString(count++, "1978.01.23");
					stmt2.setString(count++, "M");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Jack");
					stmt2.setString(count++, "1980.05.04");
					stmt2.setString(count++, "M");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Jill");
					stmt2.setString(count++, "1981.04.17");
					stmt2.setString(count++, "F");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Bell");
					stmt2.setString(count++, "1990.05.14");
					stmt2.setString(count++, "F");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();
					// movie
					stmt2.close();
					count = 1;
					stmt2 = conn.prepareStatement(
							"insert into movie(movieName,releaseYear,releaseMonth,releaseDate,publisherName,avgRate) values(?,?,?,?,?,?)");
					stmt2.setString(count++, "Edward Scissorhands");
					stmt2.setInt(count++, 1991);
					stmt2.setInt(count++, 6);
					stmt2.setInt(count++, 29);
					stmt2.setString(count++, "20th Century Fox Presents");
					stmt2.setNull(count++, Types.NUMERIC);
					stmt2.addBatch();

					count = 1;
					stmt2.setString(count++, "Alice In Wonderland");
					stmt2.setInt(count++, 2010);
					stmt2.setInt(count++, 3);
					stmt2.setInt(count++, 4);
					stmt2.setString(count++, "Korea Sony Pictures");
					stmt2.addBatch();

					count = 1;
					stmt2.setString(1, "The Social Network");
					stmt2.setInt(3, 11);
					stmt2.setInt(4, 18);
					stmt2.addBatch();

					stmt2.setString(count++, "The Dark Knight");
					stmt2.setInt(count++, 2008);
					stmt2.setInt(count++, 8);
					stmt2.setInt(count++, 6);
					stmt2.setString(count++, "Warner Brothers Korea");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();
					// genre
					stmt2.close();
					stmt2 = conn.prepareStatement("insert into genre values(?)");
					stmt2.setString(1, "Fantasy");
					stmt2.addBatch();

					stmt2.setString(1, "Romance");
					stmt2.addBatch();

					stmt2.setString(1, "Adventure");
					stmt2.addBatch();

					stmt2.setString(1, "Family");
					stmt2.addBatch();

					stmt2.setString(1, "Drama");
					stmt2.addBatch();

					stmt2.setString(1, "Action");
					stmt2.addBatch();

					stmt2.setString(1, "Mystery");
					stmt2.addBatch();

					stmt2.setString(1, "Thriller");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();
					// casting
					stmt2.close();
					stmt2 = conn.prepareStatement("insert into casting values(?,?,?)");
					stmt2.setInt(1, 1);
					stmt2.setInt(2, 1);
					stmt2.setString(3, "Main actor");
					stmt2.addBatch();

					stmt2.setInt(1, 1);
					stmt2.setInt(2, 2);
					stmt2.addBatch();

					stmt2.setInt(1, 2);
					stmt2.setInt(2, 1);
					stmt2.addBatch();

					stmt2.setInt(1, 2);
					stmt2.setInt(2, 3);
					stmt2.addBatch();

					stmt2.setInt(1, 3);
					stmt2.setInt(2, 6);
					stmt2.addBatch();

					stmt2.setInt(1, 3);
					stmt2.setInt(2, 7);
					stmt2.setString(3, "Supporting Actor");
					stmt2.addBatch();

					stmt2.setInt(1, 4);
					stmt2.setInt(2, 4);
					stmt2.setString(3, "Main actor");
					stmt2.addBatch();

					stmt2.setInt(1, 4);
					stmt2.setInt(2, 5);
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();
					// make
					stmt2.close();
					stmt2 = conn.prepareStatement("insert into make values(?,?)");
					stmt2.setInt(1, 1);
					stmt2.setInt(2, 1);
					stmt2.addBatch();

					stmt2.setInt(1, 2);
					stmt2.setInt(2, 1);
					stmt2.addBatch();

					stmt2.setInt(1, 3);
					stmt2.setInt(2, 2);
					stmt2.addBatch();

					stmt2.setInt(1, 4);
					stmt2.setInt(2, 3);
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();

					// movieGenre
					stmt2.close();
					stmt2 = conn.prepareStatement("insert into movieGenre(movieID,genreName) values(?,?)");
					stmt2.setInt(1, 1);
					stmt2.setString(2, "Fantasy");
					stmt2.addBatch();
					stmt2.setString(2, "Romance");
					stmt2.addBatch();

					stmt2.setInt(1, 2);
					stmt2.setString(2, "Fantasy");
					stmt2.addBatch();
					stmt2.setString(2, "Adventure");
					stmt2.addBatch();
					stmt2.setString(2, "Family");
					stmt2.addBatch();

					stmt2.setInt(1, 3);
					stmt2.setString(2, "Drama");
					stmt2.addBatch();

					stmt2.setInt(1, 4);
					stmt2.setString(2, "Action");
					stmt2.addBatch();
					stmt2.setString(2, "Drama");
					stmt2.addBatch();
					stmt2.setString(2, "Mystery");
					stmt2.addBatch();
					stmt2.setString(2, "Thriller");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();

					// 2번
					// 2.1. Winona Ryder won the “Best supporting actor” award in 1994
					stmt.executeUpdate("insert into award(awardName) value('Best supporting actor')");

					stmt2.close();
					stmt2 = conn.prepareStatement(
							"insert into actorObtain(awardID,actorID,year) select awardID,actorID,? from award,actor where awardName = ? and actorName = ? ");
					stmt2.setInt(1, 1994);
					stmt2.setString(2, "Best supporting actor");
					stmt2.setString(3, "Winona Ryder");
					stmt2.addBatch();

					// 2.2. Andrew Garfield won the “Best supporting actor” award in 2011
					stmt2.setInt(1, 2011);
					stmt2.setString(3, "Andrew Garfield");
					stmt2.addBatch();
					// 2.3. Jesse Eisenberg won the “Best main actor” award in 2011
					stmt.executeUpdate("insert into award(awardName) value('Best main actor')");
					stmt2.setInt(1, 2011);
					stmt2.setString(2, "Best main actor");
					stmt2.setString(3, "Jesse Eisenberg");
					stmt2.addBatch();

					// 2.4. Johnny Depp won the “Best villain actor” award in 2011
					stmt.executeUpdate("insert into award(awardName) value('Best villain actor')");
					stmt2.setString(2, "Best villain actor");
					stmt2.setString(3, "Johnny Depp");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();

					// 2.5. Edward Scissorhands won the “Best fantasy movie” award in 1991
					stmt.executeUpdate("insert into award(awardName) value('Best fantasy movie')");
					stmt2.close();
					stmt2 = conn.prepareStatement(
							"insert into movieObtain(awardID,movieID,year) select awardID,movieID,? from award,movie where awardName = ? and movieName = ? ");
					stmt2.setInt(1, 1991);
					stmt2.setString(2, "Best fantasy movie");
					stmt2.setString(3, "Edward Scissorhands");
					stmt2.addBatch();

					// 2.6. Alice In Wonderland won the “Best fantasy movie” award in 2011
					stmt2.setInt(1, 2011);
					stmt2.setString(2, "Best fantasy movie");
					stmt2.setString(3, "Alice In Wonderland");
					stmt2.addBatch();
					stmt2.executeBatch();
					stmt2.clearBatch();

					// 2번 출력
					System.out.println("<2번 문제>\n배우 수상 경력\n");
					rs = stmt.executeQuery(
							"  select * from (" + "    select awardName, actorName, year from actorObtain natural join ("
									+ "    select * from award,actor)info)result");

					System.out.println(String.format("%30s%30s%10s", "[actorName]", "[awardName]", "[year]"));
					while (rs.next()) {
						String awardName = rs.getString("awardName");
						String actorName = rs.getString("actorName");
						int year = rs.getInt("year");
						System.out.println(String.format("%30s%30s%10s", actorName, awardName, year));
					}
					rs.close();

					System.out.println("\n영화 수상 경력\n");
					rs = stmt.executeQuery(
							"  select * from (" + "    select awardName, movieName, year from movieObtain natural join ("
									+ "    select * from award,movie)info)result");

					System.out.println(String.format("%30s%30s%10s", "[movieName]", "[awardName]", "[year]"));
					while (rs.next()) {
						String awardName = rs.getString("awardName");
						String movieName = rs.getString("movieName");
						int year = rs.getInt("year");
						System.out.println(String.format("%30s%30s%10s", movieName, awardName, year));
					}
					rs.close();

					// 3.1 Bob rates 5 to “The Dark Knight”.
					stmt.executeUpdate("insert into customerRate(customerID, movieID, rate) " + "select customerID,movieID, 5 "
							+ "from customer,movie " + "where customerName = 'Bob' and movieName = 'The Dark Knight'");
					// 3.2 Bell rates 5 to the movies whose director is “Tim Burton”.
					stmt.executeUpdate("insert into customerRate(customerID, movieID, rate) " + "select customerID,movieID, 5 "
							+ "from customer,( " + "select movieID " + "from make natural join director "
							+ "where directorName = 'Tim Burton')movieID " + "where customerName = 'Bell' ");
					// 3.3 Jill rates 4 to the movies whose main actor is female.
					stmt.executeUpdate("insert into customerRate(customerID, movieID, rate) " + "select customerID,movieID, 4 "
							+ "from customer,( " + "select movieID " + "from (movie natural join casting) natural join actor "
							+ "where role = 'Main actor' and gender = 'F')movieID " + "where customerName = 'Jill'");
					// 3.4 Jack rates 4 to the fantasy movies.
					stmt.executeUpdate("insert into customerRate(customerID, movieID, rate) " + "select customerID,movieID,4 "
							+ "from customer,movieGenre " + "where customerName = 'Jack' and genreName = 'Fantasy'");

					// movie.avgRate를 업데이트 합니다.
					stmt.executeUpdate("update movie as m " + "inner join( "
							+ " select movieID,avg(rate) as avgCustomer " + "from customerRate " + "group by movieID "
							+ ") as avg " + "on m.movieID = avg.movieID " + "set m.avgRate = avg.avgCustomer");

					// 3번 출력
					System.out.println("<3번 문제> 영화별 평균 평점");
					rs = stmt.executeQuery("select movieName, avgRate " + "from movie");
					System.out.println(String.format("%30s%10s", "[movieName]", "[avgRate]"));
					while (rs.next()) {
						String movieName = rs.getString("movieName");
						float avgRate = rs.getFloat("avgRate");
						System.out.println(String.format("%30s%10s", movieName, avgRate));
					}
					rs.close();
					// 4. Select the names of the movies whose actor are dead.
					System.out.println("<4번 문제> 배우가 죽은 영화들의 목록");
					rs = stmt.executeQuery("select distinct movieName "
							+ "from movie natural join casting natural join actor " + "where dateOfDeath is not null");
					System.out.println(String.format("%30s", "[movieName]"));
					while (rs.next()) {
						String movieName = rs.getString("movieName");
						System.out.println(String.format("%30s", movieName));
					}
					rs.close();
					// 5. Select the names of the directors who cast the same actor more than once.
					System.out.println("<5번 문제> 같은 배우를 캐스팅 한 적이 있는 감독 목록");
					rs = stmt.executeQuery(
							"select distinct directorName " + "from( " + "select directorName, count(directorID) as cnt "
									+ "from movie natural join casting natural join make natural join director "
									+ "group by directorID,actorID)info " + "where cnt >1");
					System.out.println(String.format("%30s", "[directorName]"));
					while (rs.next()) {
						String directorName = rs.getString("directorName");
						System.out.println(String.format("%30s", directorName));
					}
					rs.close();
					// 6. Select the names of the movies and the genres, where movies have the
					// common genre.
					System.out.println("<6번 문제> 장르가 같은 영화 목록");
					rs = stmt.executeQuery(
							"select movieName,genreName   " + "from movie natural join (select T.movieID,T.genreName  "
									+ "from movieGenre as T ,movieGenre as S  "
									+ "where T.movieID != S.movieID and T.genreName = S.genreName)info");
					System.out.println(String.format("%30s%15s", "[movieName]", "[genreName]"));
					while (rs.next()) {
						String movieName = rs.getString("movieName");
						String genreName = rs.getString("genreName");
						System.out.println(String.format("%30s%15s", movieName, genreName));
					}
					rs.close();

					// 7. Delete the movies whose director or actor did not get any award and delete
					// data from related tables.
					stmt.executeUpdate("delete from movie " + "where movieID in ( " + "select movieID "
							+ "from casting " + "where actorID not in ( " + "select actorID " + "from actorObtain "
							+ ") " + ") ");

					System.out.println("<7번 문제> 삭제 후, movie테이블과  movie를 참조하는 테이블들의 상태\n");
					System.out.println("<<movie>>");
					rs = stmt.executeQuery("select * " + "from movie");
					System.out.println(String.format("%20s%20s%15s%15s%15s%30s%10s", "[movieID]", "[movieName]", "[releaseYear]", "[releaseMonth]", "[releaseDate]", "[publisherName]", "[avgRate]"));
					while (rs.next()) {
						String movieName = rs.getString("movieName");
						String publisherName = rs.getString("publisherName");
						int movieID = rs.getInt("movieID");
						int releaseYear = rs.getInt("releaseYear");
						int releaseMonth = rs.getInt("releaseMonth");
						int releaseDate = rs.getInt("releaseDate");
						float avgRate = rs.getFloat("avgRate");
						System.out.println(String.format("%20s%20s%15s%15s%15s%30s%10s", movieID, movieName, releaseYear, releaseMonth, releaseDate, publisherName, avgRate));
					}
					rs.close();
					System.out.println("\n<<movieGenre>>");
					rs = stmt.executeQuery("select * " + "from movieGenre");
					System.out.println(String.format("%30s%30s", "[movieID]", "[genreName]"));
					while (rs.next()) {
						int movieID = rs.getInt("movieID");
						String genreName = rs.getString("genreName");
						System.out.println(String.format("%30s%30s", movieID, genreName));
					}
					rs.close();
					System.out.println("\n<<casting>>");
					rs = stmt.executeQuery("select * " + "from casting");
					System.out.println(String.format("%30s%30s%30s", "[movieID]", "[actorID]" , "[role]"));
					while (rs.next()) {
						int movieID = rs.getInt("movieID");
						int actorID = rs.getInt("actorID");
						String role = rs.getString("role");
						System.out.println(String.format("%30s%30s%30s", movieID, actorID, role));
					}
					rs.close();
					System.out.println("\n<<make>>");
					rs = stmt.executeQuery("select * " + "from make");
					System.out.println(String.format("%30s%30s", "[movieID]", "[directorID]"));
					while (rs.next()) {
						int movieID = rs.getInt("movieID");
						int directorID = rs.getInt("directorID");
						System.out.println(String.format("%30s%30s", movieID, directorID));
					}
					rs.close();
					System.out.println("\n<<customerRate>>");
					rs = stmt.executeQuery("select * " + "from customerRate");
					System.out.println(String.format("%30s%30s%30s", "[customerID]", "[movieID]", "[rate]"));
					while (rs.next()) {
						int customerID = rs.getInt("customerID");
						int movieID = rs.getInt("movieID");
						int rate = rs.getInt("rate");
						System.out.println(String.format("%30s%30s%30s",customerID,movieID, rate));
					}
					rs.close();

					// 8. Delete all customers and delete data from related tables.
					stmt.executeUpdate("delete from customer");
					// 모든 테이블 삭제하여 초기 상태로 복구
					stmt.executeQuery("SET @tables = NULL;");
					stmt.executeQuery(" SELECT GROUP_CONCAT(table_schema, '.', table_name) INTO @tables"
							+ "   FROM information_schema.tables" + "   WHERE table_schema = 'movies';");
					stmt.executeQuery(" SET @tables = CONCAT('DROP TABLE ', @tables);");
					stmt.executeQuery("PREPARE temp FROM @tables;");
					stmt.executeQuery(" EXECUTE temp;");
					stmt.executeQuery(" DEALLOCATE PREPARE temp;");

				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					try {
						if (rs != null)
							rs.close();
					} catch (SQLException e) {
						e.printStackTrace();
					}
					try {
						if (stmt != null)
							stmt.close();
					} catch (SQLException e) {
						e.printStackTrace();
					}
					try {
						if (conn != null)
							conn.close();
					} catch (SQLException e) {
						e.printStackTrace();
					}

				}
			}
}
