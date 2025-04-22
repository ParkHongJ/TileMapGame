#pragma once
#include "UI.h"

class Image;
class Journal_1 : public UI
{
private:
	Image* imageJournalPage;
	Image* imageJournalPhoto;
	FPOINT PhotoPos;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	inline void SetInteract(bool setValue) { isInteract = setValue; };
};

