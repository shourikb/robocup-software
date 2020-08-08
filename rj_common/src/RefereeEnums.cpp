#include "rj_common/RefereeEnums.hpp"

namespace RefereeModuleEnums {

std::string string_from_stage(Stage s) { return SSL_Referee_Stage_Name(s); }

std::string string_from_command(Command c) { return SSL_Referee_Command_Name(c); }

}  // namespace RefereeModuleEnums
