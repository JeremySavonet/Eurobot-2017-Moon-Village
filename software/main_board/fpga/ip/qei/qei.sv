// ===============================
// QEI
// Jeremy Savonet
// Rev: 1.0
// November 20, 2016
// ===============================

//
// Note: Inputs quadA and quadB MUST be synchronized before entering
//       this module.

module QEI(
    input logic clk,
    input logic quadA,
    input logic quadB,
    output logic [31:0] count
);

    // Grey code states
    typedef enum logic [1:0]
    {
        S0 = 2'b00,
        S1 = 2'b01,
        S2 = 2'b11,
        S3 = 2'b10
    } stateType;

    stateType previousState;
    stateType state;
    stateType nextState;

    always_ff @ (posedge clk)
    begin
        state <= stateType'({quadA, quadB});
        previousState <= state;

    case ( {previousState, state} )
        {S0, S1}: count <= count + 1;
        {S1, S2}: count <= count + 1;
        {S2, S3}: count <= count + 1;
        {S3, S0}: count <= count + 1;
        {S2, S1}: count <= count - 1;
        {S3, S2}: count <= count - 1;
        {S0, S3}: count <= count - 1;
        {S1, S0}: count <= count - 1;
        default:    count <= count;
    endcase
    end

endmodule
