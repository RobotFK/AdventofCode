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

function Day_1_1() {
	if (Day_1_Input == undefined) {return}
	let final_height = 0;
	const heights = [];
	for (let i = 0; i < Day_1_Input.length ; i++){
		if (Day_1_Input[i] == "("){final_height += 1}
		if (Day_1_Input[i] == ")"){final_height -= 1}
		heights.push({x:i,y:final_height}); 
	}
	
	document.getElementById("output").innerHTML = String(final_height);
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
	
	document.getElementById("output").innerHTML = String(final_pos);
}