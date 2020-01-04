var React = require('react')
var ReactDOM = require('react-dom')

var Col = require('react-bootstrap/lib/Col')
var PageHeader = require('react-bootstrap/lib/PageHeader')
var Row = require('react-bootstrap/lib/Row')
var {connect} = require('react-redux')
var {reduxForm} = require('redux-form')

var DateInput = require('./DateInput')
var FormField = require('./FormField')
var LoadingButton = require('./LoadingButton')
var StaticField = require('./StaticField')
var TextInput = require('./TextInput')

var {zeroTime} = require('./utils')

var TODAY = zeroTime(new Date())

var mapStateToProps = state => state



class RegisterForm extends React.Component {
    render() {
        return (
            <form action="/" method="post">
                <fieldset>
                <div className="form-group">
                    <label for="login">Login:</label>
                    <input type="text" id="name" name="user_login"/>
                </div>
                <div className="form-group">
                    <label for="password">Hasło:</label>
                    <input type="password" id="password" name="user_password"/>
                </div>
                <div className="form-group">
                    <label for="password-again">Powtórz hasło:</label>
                    <input type="password" id="password-again" name="user_password_again"/>
                </div>
                <div className="form-group">
                    <label for="email">Email:</label>
                    <input type="email" id="email" name="user_email" />
                </div>

                <input type="submit" className="btn btn-primary" value="Zarejestruj" />
                </fieldset>
            </form>
        );
    }
}

ReactDOM.render(<RegisterForm />, document.getElementById('register'))