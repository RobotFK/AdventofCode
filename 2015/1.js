"use strict";
var Day_1_Input
document.getElementById("Day_1_Input").value = "";
document.getElementById('Day_1_Input').onchange = () => {
    const file = document.getElementById('Day_1_Input').files[0];
	if (file) {
        const reader = new FileReader();
        reader.readAsText(file, 'UTF-8');
		reader.onload = (evt) => {
          //console.log(evt.target.result);
          Day_1_Input = evt.target.result
        };
    }
};

function clear_output(){
	let node =  document.getElementById('visualisation');
	while (node.hasChildNodes()) {
	node.removeChild(node.lastChild);
	}
	node = document.getElementById('result');
	while (node.hasChildNodes()) {
	node.removeChild(node.lastChild);
	}
	return true
}


const heights = []; //Global to allow for debugging
function Day_1_1() {
	if (Day_1_Input == undefined) {return}
	let final_height = 0;
	const heights = [];
	if (heights.length < 100){//first Calc
	for (let i = 0; i < Day_1_Input.length ; i++){
		if (Day_1_Input[i] == "("){final_height += 1}
		if (Day_1_Input[i] == ")"){final_height -= 1}
		heights.push({x:i,y:final_height}); 
	}}else{
		final_height = heights[heights.length -1].y 
	}
	clear_output();
	const canvas = document.createElement('canvas');
	canvas.id = 'chart';
	canvas.style.border = '1px solid black';
	canvas.height = '500';
	canvas.width = '1500';
	document.getElementById('visualisation').appendChild(canvas);
	{ //Graph Placement
		var chartinput = {
			datasets: [{
				lable: 'height',
				data: heights
			}],
			labels: [...Array(heights.length).keys()] 
			}
		//console.log(chartinput);
		new Chart(
		document.getElementById('chart'),
		{
		  type: 'line',
		  data: chartinput,
		  options: {
			parsing: {
			  xAxisKey: 'x',
			  yAxisKey: 'y'
			},
			plugins: {
				legend: {//Not sure how to set this 
				display: false,
				},
				title: {
					display: true,
					text: 'Height of Santa'
				}
			},
		  }
		}
		);
	}
	document.getElementById("result").innerHTML = "Final height is at: "+ String(final_height);
}

function Day_1_2() {
	if (Day_1_Input == undefined) {return}
	let final_pos;
	let current_height = 0;
	const height = [];
	for (let i = 0; i < Day_1_Input.length ; i++){
		if (Day_1_Input[i] == "("){current_height += 1}
		if (Day_1_Input[i] == ")"){current_height -= 1}
		if (current_height < 0) {
			final_pos = i+1;
			break;
		}
	}
	document.getElementById("result").innerHTML = "The first Postion at witch he entered the basement is at: " + String(final_pos);
}