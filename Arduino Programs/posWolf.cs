using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Text;

public class posWolf : MonoBehaviour {

	SerialPort stream   = new SerialPort("/dev/cu.wchusbserial1410", 9600);
	int[]      ith      = new int[] {10, 11, 12, 13, 14,15,16};
	string     msg      = "0";
	float      posx     =  0 ;
	float      posy     =  0 ;
	float      posz     =  0 ;
	float      roll     =  0 ;
	float      pitch    =  0 ;
	float      yaw      =  0 ;
	Vector3    pos;
	Vector3    rot;

	// Use this for initialization
	void Start () {
		pos = transform.position;
		stream.Open();
	}
	
	// Update is called once per frame
	void Update () 
	{
		msg = stream.ReadLine();
		for(int i = 0; i < msg.Length; i++)
		{
			switch (msg.Substring(i,1))
			{
				case "z":
				ith[0] = i;
				break;
				case "r":
				ith[1] = i;
				break;
				case "p":
				ith[2] = i;
				break;
				case "w":
				ith[3] = i;
				break;
				case "a":
				ith[4] = i;
				break;
				case "b":
				ith[5] = i;
				break;
				case "c":
				ith[6] = i;
				break;
				default:
				break;
			}
		}
		roll  = float.Parse(msg.Substring(ith[0]+1, (ith[1]-ith[0]-1))     );
		pitch = float.Parse(msg.Substring(ith[1]+1, (ith[2]-ith[1]-1))     );
		yaw   = float.Parse(msg.Substring(ith[2]+1, (ith[3]-ith[2]-1))     );
		posx  = float.Parse(msg.Substring(ith[3]+1, (ith[4]-ith[3]-1))     );
		posy  = float.Parse(msg.Substring(ith[4]+1, (ith[5]-ith[4]-1))     );
		posz  = float.Parse(msg.Substring(ith[5]+1, (ith[6]-ith[5]-1))     );
		pos.x = posx;
		pos.y = posz;
		pos.z = posy;
		rot.z = (float)57.3*roll;
		rot.x = (float)57.3*pitch;
		rot.y = 180-(float)57.3*yaw;
		transform.position    = pos;
		transform.eulerAngles = rot; 
	}
}