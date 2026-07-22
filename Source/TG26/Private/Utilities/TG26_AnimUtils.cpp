// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Utilities/TG26_AnimUtils.h"

// Comparing which way the Player is Moving vs which way they are facing
float UTG26_AnimUtils::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		// Convert a rotation into a unit vector facing in its direction.
		const FVector ForwardVector = BaseRotation.Vector();
		// Same as BaseRotation.GetRightVector()
		const FVector RightVector = FRotationMatrix(BaseRotation).GetScaledAxis(EAxis::Y);
		const FVector NormalizedVelocity = Velocity.GetSafeNormal2D();
		
		// Dot Prod - is the direction we are facing similar to the velocity we are moving in
		// ArcCos - Converts dot product result to an angle in Radians
		// Then we convert to Degrees from Radians
		float ForwardDeltaDegree =  FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, NormalizedVelocity)));
		
		// Right is positive 90 degrees and left is -90 degrees. The above returns absolute values. 
		// So Dot Product of Right if Negative can be multipled by -1
		if (FVector::DotProduct(RightVector, NormalizedVelocity) < 0.0f)
		{
			ForwardDeltaDegree *= -1.f;
		}
		
		return ForwardDeltaDegree;
		
	}
	
	return 0.0f;
	
}
