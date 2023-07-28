var Day_1_Input
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
}

function Day_1_1() {
  document.getElementById("output").innerHTML = "Executed";
}