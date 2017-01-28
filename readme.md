# Open Knight Online (OpenKO)

<p align="left">
We started with project to learn more about how the MMORPG Knight Online works. MMORPGs are very intricate programs requiring knowledge in many areas of computer science such as TCP/IP, SQL server, performance tuning, 3D graphics and animation, load balancing, and more. Starting with the original leaked source we have updated to DirectX 9, wrapped window creation and player input using SDL 2.0, added function flags so that various file formats may be supported while remaining backwards compatible, and much much more.
</p>

<p align="left">
This code is for academic purposes only! If you have questions, or would like help getting started, feel free visit the <a href="http://codingbackwards.in/index.php">forums</a>.
</p>

#### Note:
* The UIE project currently requires VS15 pro
* The N3ME project currently requires VS6 to compile

#### Intentional design decisions:
* _The project is currently focused around supporting the 1298/9 version of the game_. Version 1298/9 has most of the core functionality attributed to the game’s success. By ignoring later versions of the game we keep the system relatively simplistic. This allows us to strengthen the fundamental components of the game while minimizing the amount of reverse engineering necessary to make things work.
* _We stick to the 1298/9 database schema_. To ensure compatibility with the 1298/9 version of the game we do not modify the basic database schema. This means the structure of the database and how information is stored in the database doesn’t change while we are working. This could change once the core functionality of the 1298/9 is in place.

<br>

<p align="center">
	<img src="http://stephenmeier.net/files/openko_ex_00.png" />
</p>
