var mas = [];

var data = {
  labels: [
    '0', null, null, null,
    '1', null, null, null,
    '2', null, null, null,
    '3', null, null, null,
  ],
  
  series: [
    [
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    ]
  ]
};

var options = {
  // high: 100,
  // low: 0,
  showArea: true,
  showPoint: false,
  // fullWidth: true
};



function setLabels(mas, intervalChart, quantityChart) {
  
  let now = new Date();
  let scale = [];
  let lengthChart = quantityChart * 24 * 60 / intervalChart; // 864
  
  if ( quantityChart < 10 ) {
    
    let c = 0;
    
    for (let k = 0; k < quantityChart + 1; k++) 
    for (let i = 0; i < 24; i++) { 
      
      if ( i % quantityChart == 0 || quantityChart < 1 ) scale.push((++c % 2 == 0) ? i : ''); else scale.push(null);
      for (let j = 0; j < 60 / intervalChart - 1; j++) scale.push(null);
      
    }
    
    let hh = now.getHours();
    let mm = now.getMinutes();
    
    for (let i = hh; i < 23; i++) 
    for (let j = 0; j < 60 / intervalChart; j++) scale.pop();
    
    for (let i = mm; i < 60; i++) 
    if ( i % intervalChart == 0 ) scale.pop();
    
  } else {
    
    let tt = now.getTime();
    tt -= quantityChart * 86400000;
    
    let c = 0;
    
    for (let k = 0; k < quantityChart + 1; k++) {
      
      let past = new Date(tt);
      let dd = past.getDate();
      let mmo = past.getMonth() + 1;
      let yy = past.getFullYear()
      tt += 86400000;
      let n = 14;
      let s = dd + '/' + mmo;
      if ( quantityChart > 100 ) {
        
        n = 10;
        s += '/' + yy;
        
      }
      
      if ( k % ( Math.trunc( quantityChart / n ) ) == 0 || quantityChart < 28 ) scale.push((++c % 2 == 0) ? s : ''); else scale.push(null);
      for (let j = 0; j < 24 * 60 / intervalChart - 1; j++) scale.push(null);
      
    }
    
    let hh = now.getHours();
    for (let j = hh; j < 24 * 60 / intervalChart; j++) scale.pop();
    
  }
  
  
  while ( scale.length > lengthChart ) scale.shift();
  
  // console.log(scale);
  return scale;
  
}




function setSeries(mas, intervalChart, quantityChart, c0 = null, c1 = null, c2 = null, c3 = null, c4 = null) {
  
  // console.log(c0); 
  let lengthChart = quantityChart * 24 * 60 / intervalChart; // 864
  
  let o = {};
  o.labels = setLabels(mas, intervalChart, quantityChart);
  o.series = [];
  
  let d = [];
  let m = [];
  
  Object.assign(m, mas);
  
  let chart = [c0, c1, c2, c3, c4];
  
  let now = new Date();
  let time = Math.trunc( now.getTime() / 1000 );
  
  let j = 0;
  for (let i = 0; i < lengthChart; i++) { 
    
    let obj = {};
    
    while ( m.length > 0 ) {
      
      let item = m.pop();
      
      if ( item.time > time - i * intervalChart * 60 ) {
        
        for (let item2 of Object.entries(item)) obj[item2[0]] = item2[1]; 
        continue;
        
      }
      
      if ( item.time < time - ( i + 1 ) * intervalChart * 60 ) {
        
        d.push({});
        m.push(item);
        
      } else {
        
        let obj2 = {};
        
        for (let item2 of Object.entries(item)) obj[item2[0]] = item2[1]; 
        
        // console.log(obj); 
        // console.log(item); 
        for (let k = 0; k < chart.length; k++)
        if ( chart[k] !== null ) obj2[chart[k]] = ( obj[chart[k]] !== undefined ) ? obj[chart[k]] : null;
        
        d.push(obj2);
        
      }
      
      obj = {};
      break;
      
    }
    
  }
  
  let n = lengthChart - d.length;
  for (let i = 0; i < n; i++) d.push({});
  
  d = d.reverse();
  
  for (let i = 0; i < lengthChart; i++) { 
    
    for (let k = 0; k < chart.length; k++) {
      
      if ( chart[k] !== null ) {
        if ( o.series[k] === undefined ) o.series.push([]); 
        o.series[k][i] = ( d[i][chart[k]] !== undefined ) ? d[i][chart[k]] : null;
      }
      
    }
    
  }
  
  // console.log(o);
  return o;
  
}




function parsingData(data) {
  
  let masRaw = data.split('\n');
  
  for (let item of masRaw) {
    
    let obj = parsingDataString(item);
    if ( Object.keys(obj).length !== 0 ) mas.push(obj);
    
  }
  
  mas.sort( (a, b) => a.time - b.time );
  
  // console.log(mas);
  return mas;
  
}



function parsingDataString(item) {
  
  let obj = {};
  let m = item.split(" ");
  
  if ( m[0] === '' || isNaN(Number(m[0])) ) return {};
  
  obj.time = Number(m[0]);
  
  for (let item2 of m) {
    
    let m2 = item2.split(":");
    if ( !isNaN(Number(m2[0])) || m2.length < 2 ) continue;
    obj[m2[0]] = Number(m2[1]);
    
  }
  
  return obj;
  
}



function showChart(result = null) {
  
  if ( result !== null ) mas = parsingData(result);
  
  // Промежуток между измерениями на графике, минут
  
  let iDay = 5;  
  let iMonth = 60; 
  let iYear = 360;
  
  // Длительность графика, дней
  
  let dDay = 2;
  let dMonth = 30;
  let dYear = 200;
  
  // options.low = null;
  // options.high = null;
  options.low = 10;
  options.high = 40;
  
  Chartist.Line('.ct-chart-td', setSeries(mas, iDay, dDay, 't'), options);
  Chartist.Line('.ct-chart-tm', setSeries(mas, iMonth, dMonth, 't'), options);
  Chartist.Line('.ct-chart-ty', setSeries(mas, iYear, dYear, 't'), options);
  
  options.low = -10;
  options.high = 30;
  
  Chartist.Line('.ct-chart-std', setSeries(mas, iDay, dDay, 'st'), options);
  Chartist.Line('.ct-chart-stm', setSeries(mas, iMonth, dMonth, 'st'), options);
  Chartist.Line('.ct-chart-sty', setSeries(mas, iYear, dYear, 'st'), options);
  
}



function getData(url = '/db/d0.csv') {
  
  fetch(url)
  .then(response => response.text())
  .then(result => showChart(result))
  .catch(error => console.log(error.message));
  
}

getData('/db/d0.csv?t=DataRow15');
getData('/db/d1.csv');
getData('/db/d2.csv');
getData('/db/m0.csv?t=DataRow15');
getData('/db/m1.csv');
getData('/db/m2.csv');
getData('/db/y0.csv?t=DataRow15');
getData('/db/y1.csv');
getData('/db/y2.csv');

setInterval(function() { getData('/db/d0.csv?t=DataRow15'); }, 60000);

setInterval(function() {
  
  fetch('/data?t=DataRow14')
  .then(response => response.text())
  .then(result => {
    
    let obj = parsingDataString(result);
    
    if ( obj.time !== null ) {
      let elem = document.getElementById("time");
      let now = new Date( obj.time * 1000 );
      let h = ( now.getHours() < 10 ) ? "0" + now.getHours() : now.getHours();
      let m = ( now.getMinutes() < 10 ) ? "0" + now.getMinutes() : now.getMinutes();
      let s = ( now.getSeconds() < 10 ) ? "0" + now.getSeconds() : now.getSeconds();
      elem.innerHTML = h + ":" + m + ":" + s;
    }
    
    
    if ( obj['t'] !== undefined ) {
      let elem = document.getElementById("temp");
      elem.innerHTML = obj['t'].toFixed(1) + " C°";
    }
    
    if ( obj['st'] !== undefined ) {
      let elem = document.getElementById("stemp");
      elem.innerHTML = obj['st'].toFixed(1) + " C°";
    }
    
  })
  .catch(error => console.log(error.message));
  
}, 1000);

