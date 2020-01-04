import React, { Component } from 'react';
import Select from 'react-select';
import 'react-select/dist/react-select.css';
import { Charts, ChartContainer, ChartRow, YAxis, LineChart }
from "react-timeseries-charts";
import { TimeSeries } from "pondjs";
import logo from './logo.svg';
import './App.css';


class FlowerChart extends Component {
  constructor(props) {
    super(props);
    this.state = {data: props.data};
  }

  render() {
    const data = this.state.data;
    const column_name = data.columns().toString();

    return (
      <ChartContainer timeRange={data.range()} format="%m-%d %H:%M"
                      enablePanZoom width={1200}>
        <ChartRow height="200">
          <YAxis id={column_name} label={data.name()}
                 min={data.min(column_name)}
                 max={data.max(column_name)}
                 width="100" type="linear"
                 format=","/>
          <Charts>
            <LineChart axis={column_name} series={data}
                       columns={[column_name]}
                       interpolation = "curveBasis" />
          </Charts>
        </ChartRow>
      </ChartContainer>
    );
  }
}


class FlowerPulse extends Component {
  constructor(props) {
    super(props);
    this.state = {
      selection: null,
      flowers: [],
      temperature: undefined,
      moisture: undefined,
      light: undefined,
    };

    this.flowersList = this.flowersList.bind(this);
    this.handleSelection = this.handleSelection.bind(this);

    this.temperatureSeries = this.temperatureSeries.bind(this);
    this.moistureSeries = this.moistureSeries.bind(this);
    this.lightSeries = this.lightSeries.bind(this);

    this.obtainFlowerData = this.obtainFlowerData.bind(this);
  }

  handleSelection = (selectedOption) => {
    this.setState({ selection: selectedOption },
      () => {
        this.obtainFlowerData();
      });
    if (selectedOption)
      console.log(`Selected: ${selectedOption.label}`);
  }

  flowersList() {
    fetch(`http://localhost:48088/backend/iot/flowers`)
      .then(result=> {
        return result.json();
      }).then(data=> {
        let flowers = data.map(item=> {
          return {value: item.name, label: item.name};
        });
        this.setState({flowers: flowers});
      });
  }

  obtainFlowerData() {
    if (this.state.selection) {
      this.temperatureSeries();
      this.moistureSeries();
      this.lightSeries();
    } else {
      this.setState({
        light: undefined,
        moisture: undefined,
        temperature: undefined
      });
    }
  }

  temperatureSeries() {
    fetch(`http://localhost:48088/backend/iot/ts` +
    `?flower=${this.state.selection.value}`)
      .then(result=> {
        return result.json();
      }).then(data => {
        let points = data.map(item=> {
          return [item.time, item.temperature];
        });

        if (points && points.length) {
          this.setState({
            temperature: new TimeSeries({
              name: "Temperature",
              columns: ["time", "temp"],
              points: points,
            })
          });
        }
    });
  }

  moistureSeries() {
    fetch(`http://localhost:48088/backend/iot/ms` +
    `?flower=${this.state.selection.value}`)
      .then(result=> {
        return result.json();
      }).then(data=> {
      let points = data.map(item=> {
        return [item.time, item.moisture];
      });

      if (points && points.length) {
        this.setState({
          moisture: new TimeSeries({
            name: "Moisture",
            columns: ["time", "moisture"],
            points: points,
          })
        });
      }
    });
  }

  lightSeries() {
    fetch(`http://localhost:48088/backend/iot/ls` +
    `?flower=${this.state.selection.value}`)
      .then(result=> {
        return result.json();
      }).then(data=> {
      let points = data.map(item=> {
        return [item.time, item.intensity];
      });

      if (points && points.length) {
        this.setState({
          light: new TimeSeries({
            name: "Light intensity",
            columns: ["time", "light"],
            points: points,
          })
        });
      }
    });
  }

  componentDidMount() {
    var refresh = 5000;
    this.flowersList();
    this.obtainFlowerData();
    this.interval = setInterval(this.obtainFlowerData, refresh);
  }

  componentWillUnmount() {
    clearInterval(this.interval);
  }

  render() {
    let tempChart = undefined;
    let moistureChart = undefined;
    let lightChart = undefined;

    if (this.state.temperature)
      tempChart = <FlowerChart data={this.state.temperature}/>;

    if (this.state.moisture)
      moistureChart = <FlowerChart data={this.state.moisture}/>;

    if (this.state.light)
      lightChart = <FlowerChart data={this.state.light}/>;

    return (
      <section className="FlowerPulse">
        <header>
          <h1>Check if your flowers are healthy</h1>
        </header>
        <div>
          <Select
            name="flower-name"
            value={this.state.selection}
            onChange={this.handleSelection}
            options={this.state.flowers}
          />
        </div>

        {tempChart}
        {moistureChart}
        {lightChart}

      </section>
    );
  }
}

class AlarmManager extends Component {
  constructor(props) {
    super(props);
    this.state = { turned_off: false, alarms: []};

    this.alarmsList = this.alarmsList.bind(this);
    this.sendAlarm = this.sendAlarm.bind(this);
    this.fireAlarm = this.fireAlarm.bind(this);
  }

  alarmsList() {
    fetch(`http://localhost:48088/backend/iot/alarms`)
      .then(results=> {
        return results.json();
      }).then(data=> {
        let alarms = data.map(item => {
          return (
          <tr key={item.pk}>
            <td>{item.time}</td>
            <td>{item.reason}</td>
            <td>{item.is_automatic.toString()}</td>
          </tr>)
        });
        this.setState({alarms: alarms});
      });
  }

  sendAlarm() {
    fetch('http://localhost:48088/backend/iot/alarms', {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        is_automatic: true,
        reason: 'Triggered by user',
      }),
    }).then(results=> {
      this.alarmsList();
    });
  }

  componentDidMount() {
    this.alarmsList();
  }

  fireAlarm() {
    this.sendAlarm();
  }

  render() {
    return (
      <section className="Alarms">
        <header>
          <h1>Alarms management</h1>
        </header>
        <div>
          <span><button type="button" onClick={this.fireAlarm}>Fire alarm</button></span>
          <span>
            <table>
              <thead>
                <tr>
                  <th>Time</th>
                  <th>Trigger reason</th>
                  <th>Automatic</th>
                </tr>
              </thead>
              <tbody>
                {this.state.alarms}
              </tbody>
            </table>
          </span>
        </div>
      </section>
    );
  }
}

class App extends Component {
  constructor(props) {
    super(props);
    this.state = { alarm_mgr: AlarmManager,
      pulse_chart: FlowerPulse};
  }

  render() {
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h1 className="App-title">Welcome to Flower Health Center</h1>
          <nav>

          </nav>
        </header>
        <p className="App-intro">
          Monitor status of your flowers. Run alarms. Configure your
          own flower health center!
        </p>
        <main>
          {React.createElement(this.state.pulse_chart)}
          {React.createElement(this.state.alarm_mgr)}
        </main>
      </div>
    );
  }
}

export default App;
