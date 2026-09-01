#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min() : m_min{std::numeric_limits<double>::max()} {
	}

	void update(double next) override {
		if (next < m_min) {
			m_min = next;
		}
	}

	double eval() const override {
		return m_min;
	}

	const char * name() const override {
		return "min";
	}

private:
	double m_min;
};


class Max : public IStatistics {
public:
	Max() : m_max{std::numeric_limits<double>::min()} {};

	void update(double next) override {
		if (next > m_max) {
			m_max = next;
		}
	}

	double eval() const override {
		return m_max;
	}

	const char* name() const override {
		return "max";
	}

private:
	double m_max;
};

class Mean : public IStatistics {
public:
	Mean() : num_elem{0}, m_sum{0} {};

	void update(double next) override {
		num_elem++;
		m_sum += next;
	}

	double eval() const override {
		if (num_elem == 0) {
			return 0.0;
		}
		return m_sum / num_elem;
	}

	const char* name() const override {
		return "mean";
	}

private:
	size_t num_elem;
	double m_sum;
};

class StandartDeviation : public IStatistics {
public:
	StandartDeviation() : num_elem{0}, m_sum{0}, m_square_sum{0} {};

	void update(double next) override {
		num_elem++;
		m_sum += next;
		m_square_sum += std::pow(next, 2);
	}

	double eval() const override {
		if (num_elem == 0) {
			return 0.0;
		}

		return std::sqrt(m_square_sum / num_elem - std::pow(m_sum / num_elem, 2));
	}

	const char* name() const override {
		return "std";
	}
private:
	size_t num_elem;
	double m_sum;
	double m_square_sum;
};

class Percentile90 : public IStatistics {
public: 
	Percentile90() {};

	void update(double next) override {
		m_vector.push_back(next);
	}

	double eval() const override {

		std::vector<double> sorted_m_vector = m_vector;

		std::sort(sorted_m_vector.begin(), sorted_m_vector.end());
		
		double index = 0.9 * (m_vector.size() - 1);

		size_t real_index = static_cast<size_t>(std::round(index));
		
		return m_vector[real_index];

	}

	const char* name() const override {
		return "pct90";
	}

private:
	std::vector<double> m_vector; 
};

class Percentile95 : public IStatistics {
public: 
	Percentile95() {};

	void update(double next) override {
		m_vector.push_back(next);
	}

	double eval() const override {

		std::vector<double> sorted_m_vector = m_vector;

		std::sort(sorted_m_vector.begin(), sorted_m_vector.end());

		double index = 0.95 * (m_vector.size() - 1);

		size_t real_index = static_cast<size_t>(std::round(index));
		
		return m_vector[real_index];

	}

	const char* name() const override {
		return "pct90";
	}

private:
	std::vector<double> m_vector; 
};

int main() {

	const size_t statistics_count = 6;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Max{};
	statistics[2] = new Mean{};
	statistics[3] = new StandartDeviation{};
	statistics[4] = new Percentile90{};
	statistics[5] = new Percentile95{};

	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}