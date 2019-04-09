class Integer {
	int value;
	static int width;
public:
	Integer(int _value, int _width = Integer::width) {
		this->value = _value;
		this->width = _width;
	}
	int getValue() { return this->value; }
	void setValue(int _value) { this->value = _value; }
	int getWidth() { return this->width; }
	void setWidth(int _width) { this->width = _width; }
	friend ostream& operator<<(ostream& out, Integer& obj);
	int operator= (int _value) { return this->value = _value; }
	Integer operator+(Integer &rightHandSide) {	return Integer(this->value + rightHandSide.value); }
	Integer operator+(int rightHandSide) { return Integer(this->value + rightHandSide); }
	template <typename T>
	T operator+(T rightHandSide) { return this->value + rightHandSide; }
};

int Integer::width = 4;

ostream& operator<<(ostream& out, Integer& obj) {
	string ostring = to_string(obj.value);
	while (ostring.length() < Integer::width) ostring.insert(ostring.begin(), '0');
	out << ostring;
	return out;
}