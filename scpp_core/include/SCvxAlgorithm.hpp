#pragma once

#include "activeModel.hpp"
#include "ecosWrapper.hpp"
#include "parameterServer.hpp"
#include "trajectoryData.hpp"
namespace scpp
{

class SCvxAlgorithm
{
public:
    /**
     * @brief Construct a new SC solver.
     * 
     * @param model     The system model.
     */
    explicit SCvxAlgorithm(Model::ptr_t model);

    /**
     * @brief Initializes the algorithm. Has to be called before solving the problem.
     * 
     */
    void initialize();

    /**
     * @brief Solves the system.
     * 
     * @param warm_start    Whether to reuse the last computed trajectory.
     */
    void solve(bool warm_start = false);

    /**
     * @brief Get the solution variables object.
     * 
     * @param X     The state trajectory.
     * @param U     The input trajectory.
     * @param t     The final time.
     */
    void getSolution(Model::state_vector_v_t &X, Model::input_vector_v_t &U, double &t);

    /**
     * @brief Get the solution from each iteration
     * 
     * @param X 
     * @param U 
     */
    void getAllSolutions(std::vector<Model::state_vector_v_t> &X,
                         std::vector<Model::input_vector_v_t> &U,
                         std::vector<double> &t);

private:
    /**
     * @brief Saves solution indices for performance.
     * 
     */
    void cacheIndices();

    /**
     * @brief Reads the solution variables X, U.
     * 
     */
    void readSolution();

    /**
     * @brief Loads the parameters from the configuration file.
     * 
     */
    void loadParameters();

    /**
     * @brief Performs a Successive Convexification iteration.
     * 
     */
    bool iterate();

    /**
     * @brief Get the nonlinear cost by integrating the dynamics
     * 
     * @return double 
     */
    double getNonlinearCost();

    size_t K;

    Model::ptr_t model;

    bool interpolate_input;

    bool nondimensionalize;
    double alpha;
    double beta;
    double rho_0;
    double rho_1;
    double rho_2;
    double change_threshold;

    double trust_region;
    double weight_virtual_control;
    size_t max_iterations;
    std::optional<double> last_nonlinear_cost;

    TrajectoryData td;

    double sigma;
    Model::state_vector_v_t X;
    Model::input_vector_v_t U;

    std::vector<Model::state_vector_v_t> all_X;
    std::vector<Model::input_vector_v_t> all_U;

    Eigen::MatrixXi X_indices;
    Eigen::MatrixXi U_indices;

    op::SecondOrderConeProgram socp;

    std::unique_ptr<EcosWrapper> solver;
};

} // namespace scpp