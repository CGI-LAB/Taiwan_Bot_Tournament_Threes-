CGI-Threes
========

CGI-Threes is an AI program for Game Threes!. This version is exactly the one used to participate in Taiwan 2048-Bot Tournament, http://2048-botcontest.twbbs.org/. The version was ranked the second in the tournament, as shown in http://2048-botcontest.twbbs.org/download/stats_Threes.htm.

Our algorithm is based on the following two papers: 

[1] Szubert, M. amd Jaskowaski, W., “Temporal Difference Learning of N-tuple Networks for the Game 2048”, IEEE  CIG 2014 conference, August 2014.

[2] I-Chen Wu, Kun-Hao Yeh, Chao-Chin Liang, Chia-Chuan Chang, and Han Chiang, "Multi-Stage Temporal Difference Learning for 2048", the 2014 Conference on Technologies and Applications of Artificial Intelligence (TAAI 2014), Taipei, Taiwan, Novemeber 2014.

Since we don't have much time to refactor the code and write up documents, we simply leave the way it is (it could be hard to read). To run the program, use our trained data which can be downloaded from http://aigames.nctu.edu.tw/~cgilab/download/. 


Performance
========

<table align=center>
	<tr align=center> 
		<th>Indicators</th> 
		<th>CGI-Threes</th>
	</tr>
	
	<tr align=center> 
		<td>384</td> 
		<td>100.0%</td> 
	</tr> 
	<tr align=center> 
		<td>768</td> 
		<td>100.0%</td> 
	</tr> 
	<tr align=center> 
		<td>1536</td> 
		<td>96.0%</td> 
	</tr> 
	<tr align=center> 
		<td>3072</td> 
		<td>68.0%</td>
	</tr> 
	<tr align=center> 
		<td>6144</td> 
		<td>10.0%</td> 
	</tr> 
	<tr align=center> 
		<td>Max score</td> 
		<td>619347</td>
	</tr> 
	<tr align=center> 
		<td>Ave score</td> 
		<td>223595</td>
	</tr> 
	<tr align=center> 
		<td>Speed</td> 
		<td>about 500 moves/sec</td>
	</tr> 
</table>

In the above table, we show the performance of CGI-Threes in the Taiwan 2048-Bot Tournament.
